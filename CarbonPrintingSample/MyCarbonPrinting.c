/*
	File:		MyCarbonPrinting.c	
	Contains:	Routines needed to perform printing. This example uses sheets and provides for
                        save as PDF and save as PostScript.

	Copyright © 1999-2007 Apple Inc. All Rights Reserved

	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Inc.
				("Apple") in consideration of your agreement to the following terms, and your
				use, installation, modification or redistribution of this Apple software
				constitutes acceptance of these terms.  If you do not agree with these terms,
				please do not use, install, modify or redistribute this Apple software.

				In consideration of your agreement to abide by the following terms, and subject
				to these terms, Apple grants you a personal, non-exclusive license, under AppleÕs
				copyrights in this original Apple software (the "Apple Software"), to use,
				reproduce, modify and redistribute the Apple Software, with or without
				modifications, in source and/or binary forms; provided that if you redistribute
				the Apple Software in its entirety and without modifications, you must retain
				this notice and the following text and disclaimers in all such redistributions of
				the Apple Software.  Neither the name, trademarks, service marks or logos of
				Apple Inc. may be used to endorse or promote products derived from the
				Apple Software without specific prior written permission from Apple.  Except as
				expressly stated in this notice, no other rights or licenses, express or implied,
				are granted by Apple herein, including but not limited to any patent rights that
				may be infringed by your derivative works or by other works in which the Apple
				Software may be incorporated.

				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
				WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
				WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
				PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
				COMBINATION WITH YOUR PRODUCTS.

				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
				CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
				GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
				ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION
				OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
				(INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN
				ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "MyCarbonPrinting.h"
#include "AppDrawing.h"
#include "UIHandling.h"
#include "PDECommon.h"

/*** Local Function Prototypes ***/

static void MyPageSetupDoneProc(PMPrintSession printSession, 
                                            WindowRef documentWindow, Boolean accepted);
static void MyPrintDialogDoneProc(PMPrintSession printSession, 
                                            WindowRef documentWindow, Boolean accepted);

static OSStatus MyDoPrintLoop(PMPrintSession printSession, PMPageFormat pageFormat, 
				    PMPrintSettings printSettings, const void *docDataP,
                                    Boolean doStatusDialog);

// -----------------------------------------------------------------------
OSStatus CreateDefaultPageFormatForDocument(void *docDataP)
{
    OSStatus err = noErr, tempErr;
    PMPrintSession printSession;
    err = PMCreateSession(&printSession);
    if(!err){
	PMPageFormat pageFormat;
	err = PMCreatePageFormat(&pageFormat);	// we own a reference to this page format
	if(err == noErr)
	{
	    err = PMSessionDefaultPageFormat(printSession, pageFormat);
	    if (err == noErr){	
		// this routine will keep its own reference to the pageFormat
		err = SetPageFormatOnPrivateData(docDataP, pageFormat);	
	    }
	    tempErr = PMRelease(pageFormat);// release our reference obtained from PMCreatePageFormat
	    if(!err)err = tempErr;
	}
	tempErr = PMRelease(printSession);
	if(!err)err = tempErr;
    }
    return err;
}

// -----------------------------------------------------------------
OSStatus DoPageSetup(WindowRef window, void *docDataP)
{
    OSStatus		err = noErr;
    if(docDataP){
        PMPrintSession printSession;
        err = PMCreateSession(&printSession);
		if(!err){
			Boolean accepted = true;
			static PMSheetDoneUPP myPageSetupDoneProc = NULL;
			if(myPageSetupDoneProc == NULL){
				myPageSetupDoneProc = NewPMSheetDoneUPP(MyPageSetupDoneProc);
				if(!myPageSetupDoneProc){
					err = memFullErr;
				}
			}
				
			if(!err) 	// validate the page format we're going to pass to the dialog code
				err = PMSessionValidatePageFormat(printSession, GetPageFormatFromPrivateData(docDataP),
									kPMDontWantBoolean);


			if(!err){
				Boolean doSheets = useSheets();
				
				// If we are doing sheets and PMShowPageSetupDialogAsSheet is available, use it.
				if(doSheets && &PMShowPageSetupDialogAsSheet != NULL){
					err = PMShowPageSetupDialogAsSheet(printSession, GetPageFormatFromPrivateData(docDataP),
														window, myPageSetupDoneProc);
				}else{
					// If we are doing sheets and PMShowPageSetupDialogAsSheet isn't available, use older API 
					if(doSheets)
						err = PMSessionUseSheets(printSession, window, myPageSetupDoneProc);
								
					if(!err)
						err = PMSessionPageSetupDialog(printSession, GetPageFormatFromPrivateData(docDataP), &accepted);
				}
				/*  When using sheets, the value of 'accepted' returned here is irrelevant
					since it is our sheet done proc that is called when the dialog is dismissed.
					Our dialog done proc will be called when the sheet is dismissed.
					
					If we aren't using sheets then we call our DialogDone proc here 
					to complete the dialog.
				*/
				if(err == noErr && !doSheets)
					MyPageSetupDoneProc(printSession, window, accepted);
			}
			if(err){	// only if there is an error do we release the session,
						// otherwise we'll do that in our sheet done proc
				(void)PMRelease(printSession);
			}
		}
    }
    DoErrorAlert(err, kMyPrintErrorFormatStrKey);
    return err;
} // DoPageSetup

// -------------------------------------------------------------------------------
static void MyPageSetupDoneProc(PMPrintSession printSession,
                        WindowRef documentWindow,
                        Boolean accepted)
{
#pragma unused(documentWindow, accepted)
    // this sample doesn't do anything with the page format after page setup is done
    
    // now we release the session we created to do the page setup dialog
    OSStatus err = PMRelease(printSession);	
    if(err)
        DoErrorAlert(err, kMyPrintErrorFormatStrKey);
    return;
}


// -------------------------------------------------------------------------------
OSStatus DoPrint(WindowRef parentWindow, void *documentDataP, Boolean printOne)
{
    OSStatus err = noErr;
    PMPrintSettings printSettings = NULL;

    UInt32 minPage = 1, maxPage;
    PMPrintSession printSession;
    err = PMCreateSession(&printSession);
	if(err == noErr){
		// validate the page format we're going to use
		err = PMSessionValidatePageFormat(printSession, 
					GetPageFormatFromPrivateData(documentDataP),
					kPMDontWantBoolean);
		if (err == noErr)
		{
			err = PMCreatePrintSettings(&printSettings);
			if(err == noErr)
			{
				err = PMSessionDefaultPrintSettings(printSession, printSettings);
				if(err == noErr){
					CFStringRef myDocumentNameRef;
					err = CopyDocumentName(documentDataP, &myDocumentNameRef);
					if(err == noErr)
					{
						err = PMPrintSettingsSetJobName(printSettings, myDocumentNameRef);
						CFRelease(myDocumentNameRef);	// release our reference to the document name
					}
				}
			}
			if (err == noErr)
			{
				/*
					Calling PMSetPageRange has the following benefits:
					(a) sets the From/To settings in the print dialog to the range of pages 
						in the document 
						
						AND 
						
					(b) the print dialog code enforces this so that users can't enter 
						values outside this range.
				*/
				maxPage = GetMyDocumentNumPagesInDoc(documentDataP);
				err = PMSetPageRange(printSettings, minPage, maxPage);
			}

			// Are new API calls available? Boolean is set true if not.
			Boolean useTigerCompatibleAPIs = &PMShowPrintDialogWithOptions == NULL;

			if (err == noErr)
			{
				Boolean accepted = true;
				static PMSheetDoneUPP myPrintDialogDoneProc = NULL;
				if(myPrintDialogDoneProc == NULL){
					myPrintDialogDoneProc = NewPMSheetDoneUPP(MyPrintDialogDoneProc);
					if(!myPrintDialogDoneProc)
						err = memFullErr;
				}

				if(!err){
					Boolean doSheets = useSheets();
					err = SetPrintSettingsOnPrivateData(documentDataP, printSettings);
					if(!err){
						if(!printOne){
							if(!useTigerCompatibleAPIs){
								PMPrintDialogOptionFlags flags = showInlineDefaults()? kPMShowDefaultInlineItems: kPMHideInlineItems;								
								if ( showPageAttributes() )
									flags |= kPMShowInlinePaperSize;		// or if you want a separate PDE use kPMShowPageAttributesPDE
								if ( showOrientation() )
									flags |= kPMShowInlineOrientation;

								PMPageFormat format = GetPageFormatFromPrivateData(documentDataP);
								if(doSheets){
									err = PMShowPrintDialogWithOptionsAsSheet(printSession, printSettings, 
												format, flags,
												parentWindow, myPrintDialogDoneProc);
								}else{
									err = PMShowPrintDialogWithOptions(printSession, printSettings, 
												format, flags, 
												&accepted);
								}
								if(err != noErr)
									fprintf(stderr, "Got error %d when trying to show print dialog.\n", (int)err);
							}else{
								if(doSheets)
									err = PMSessionUseSheets(printSession, parentWindow, myPrintDialogDoneProc);
								
								if(!err)
									err = PMSessionPrintDialog(printSession, printSettings, 
											GetPageFormatFromPrivateData(documentDataP),
											&accepted);
							}
						
							/*  
								If we aren't using sheets then we call our DialogDone proc here 
								to complete the dialog.
							*/
							if(err == noErr && !doSheets)
								MyPrintDialogDoneProc(printSession, parentWindow, accepted);
						}else{
							// if we are doing print one we have no dialog, therefore
							// we have to call our dialog done proc since there is no
							// dialog to do so for us
							if(err == noErr)
								MyPrintDialogDoneProc(printSession, parentWindow, true);
						}
					}
				}
			}
		}
		// need to release the print settings this function created.
		if(printSettings){
			OSStatus tempErr = PMRelease(printSettings);
			if(err == noErr)
				err = tempErr;
			printSettings = NULL;
		}
		if(err != noErr){
			/* normally the printSettings set in the Private Data and printSession would be released 
			by our dialog done proc but if we got an error and therefore got to this point in our code, 
			the dialog done proc was NOT called and therefore we need to release the
			printSession here
			*/
			if(printSettings){
				// this releases any print settings already stored on our private data
				(void)SetPrintSettingsOnPrivateData(documentDataP, NULL);
			}
			(void)PMRelease(printSession);   // ignoring error since we already have one 
		}
	}
    
    DoErrorAlert(err, kMyPrintErrorFormatStrKey);
    return err;
}

// ---------------------------------------------------------------------------------------------
static void MyPrintDialogDoneProc(PMPrintSession printSession,
                            WindowRef documentWindow, Boolean accepted)
{
    OSStatus err = noErr, tempErr;
    void *ourDataP = GetMyWindowProperty(documentWindow);
    if(ourDataP)
    {
		// only run the print loop if the user accepted the print dialog.
        if(accepted){
            err = MyDoPrintLoop(printSession, 
                        GetPageFormatFromPrivateData(ourDataP),
                        GetPrintSettingsFromPrivateData(ourDataP), 
						ourDataP, true);
		}

        // We're done with the print settings on our private data so
        // set them to NULL which causes the print settings to be released
        tempErr = SetPrintSettingsOnPrivateData(ourDataP, NULL);
        if(err == noErr)
            err = tempErr;
    }// else Assert(...);
    
    // now we release the session we created to do the Print dialog
    tempErr = PMRelease(printSession);
    if(err == noErr)
        err = tempErr;
    
    DoErrorAlert(err, kMyPrintErrorFormatStrKey);
}

// --------------------------------------------------------------------------------------
static OSStatus MyDoPrintLoop(PMPrintSession printSession, PMPageFormat pageFormat, 
				PMPrintSettings printSettings, const void *ourDataP,
                                Boolean doPrintingStatusDialog)
{
    OSStatus err = noErr;
    OSStatus tempErr = noErr;
    UInt32 firstPage, lastPage, totalDocPages = GetMyDocumentNumPagesInDoc(ourDataP);
	CFBooleanRef settingsCFBool;
	Boolean doPrintTitles;

	// This code doesn't do anything to use the doPrintTitles setting but just shows how to get the
	// data set in the custom printing pane.
	err = PMPrintSettingsGetValue(printSettings, kMyApplicationPrintSettingsKey, (CFTypeRef*)&settingsCFBool);
	if(!err){
		doPrintTitles = CFBooleanGetValue(settingsCFBool);
	}else{
		doPrintTitles = kPrintTitlesOnlyDefault;
		err = noErr;
	}
    
    if(!err)
	err = PMGetFirstPage(printSettings, &firstPage);
	
    if (!err)
        err = PMGetLastPage(printSettings, &lastPage);

    if(!err && lastPage > totalDocPages){
        // don't draw more than the number of pages in our document
        lastPage = totalDocPages;
    }

    if (!err)		// tell the printing system the number of pages we are going to print
        err = PMSetLastPage(printSettings, lastPage, false);

    //	Note: we don't have to worry about the number of copies.  The printing
    //	manager handles this.  So we just iterate through the document from the
    //	first page to be printed, to the last.
    if (!err)
    {
        PageDrawProc *drawProc = GetMyDrawPageProc(ourDataP);

		if(doPrintingStatusDialog)
			err = PMSessionBeginCGDocument(printSession, printSettings, pageFormat);
		else
			err = PMSessionBeginCGDocumentNoDialog(printSession, printSettings, pageFormat);
	
		if (!err){
			UInt32 pageNumber = firstPage;
			PMPaper paper;
			CGRect cgRect;
			double width, height;
			PMOrientation orientation = kPMPortrait;
			
			err = PMGetPageFormatPaper(pageFormat, &paper);
			if(!err)
				err = PMPaperGetWidth(paper, &width);
			if(!err)
				err = PMPaperGetHeight(paper, &height);
			if(!err)
				err = PMGetOrientation(pageFormat, &orientation);
			
			if(!err && (orientation == kPMLandscape || orientation == kPMReverseLandscape) ){
				// Landscape paper sizes have the width and height reversed.
				double temp;
				temp = height;
				height = width;
				width = temp;
			}
			
			// This rectangle describes the sheet we are drawing to.
			cgRect = CGRectMake(0, 0, width, height);
			
			// need to check errors from our print loop and errors from the session for each
			// time around our print loop before calling BeginPage
			while(pageNumber <= lastPage && err == noErr && PMSessionError(printSession) == noErr)
			{
				if(doPrintingStatusDialog)
					err = PMSessionBeginPage(printSession, pageFormat, NULL);
				else
					err = PMSessionBeginPageNoDialog(printSession, pageFormat, NULL);

				if (!err){
					CGContextRef context;
					err = PMSessionGetCGGraphicsContext(printSession, &context);
					if(!err){
						err = drawProc(ourDataP, context, &cgRect, pageNumber); // image the correct page
					}
					// We must call EndPage if BeginPage returned noErr
				
					if(doPrintingStatusDialog)
						tempErr = PMSessionEndPage(printSession);
					else
						tempErr = PMSessionEndPageNoDialog(printSession);
							
					if(!err)err = tempErr;
				}
				pageNumber++;
			}	// end while loop
				
			if(doPrintingStatusDialog)
				// we must call EndDocument if BeginDocument returned noErr
				tempErr = PMSessionEndDocument(printSession);
			else
				tempErr = PMSessionEndDocumentNoDialog(printSession);
		
			if(!err)err = tempErr;
			
			if(!err)
				err = PMSessionError(printSession);
		}
    }
    return err;
}

OSStatus MakePDForPSDocument(WindowRef parentWindow, void *documentDataP, CFURLRef saveURL, Boolean doPostScript)
{
#pragma unused (parentWindow)
    OSStatus err = noErr, tempErr;
    PMPrintSettings printSettings = NULL;
    PMPrintSession printSession;
    err = PMCreateSession(&printSession);
    if(err == noErr){
		// validate the page format we're going to use
		err = PMSessionValidatePageFormat(printSession, 
			    GetPageFormatFromPrivateData(documentDataP),
			    kPMDontWantBoolean);
        if (err == noErr)
        {
            err = PMCreatePrintSettings(&printSettings);
            if(err == noErr)
            {
                err = PMSessionDefaultPrintSettings(printSession, printSettings);
                if(err == noErr){
                    CFStringRef myDocumentNameRef;
                    err = CopyDocumentName(documentDataP, &myDocumentNameRef);
                    if(err == noErr)
                    {
                        err = PMPrintSettingsSetJobName(printSettings, myDocumentNameRef);
                        CFRelease(myDocumentNameRef);	// release our reference to the document name
                    }
                }

                if (err == noErr)
                    err = PMSetPageRange(printSettings, 1, GetMyDocumentNumPagesInDoc(documentDataP));
                
                // set our destination to be our target URL and the format to be PDF or PostScript
				// depending on the request.
                if(err == noErr){
                    err == PMSessionSetDestination(printSession, printSettings,
                                    kPMDestinationFile, 
				    doPostScript ? kPMDocumentFormatPostScript : kPMDocumentFormatPDF, 
                                    saveURL);
                }
                /*
                    It looks a bit odd to call PMSessionUseSheets when we aren't even
                    using the print dialog BUT the reason we are doing so is so that
                    the printing status narration will use sheets and not a modal
                    dialog. NOTE: this is only called when showStatusDialogOnSave
					returns true, that is we want status narration.
                */
                if(err == noErr && showStatusDialogOnSave()){
                    err = PMSessionUseSheets(printSession, parentWindow, NULL);             
                }
            }

            if (err == noErr)
            {
                err = MyDoPrintLoop(printSession, 
                            GetPageFormatFromPrivateData(documentDataP),
                            printSettings, 
                            documentDataP,
							showStatusDialogOnSave()
						);
            }
        }
        if(printSettings){
            tempErr = PMRelease(printSettings);
            if(err == noErr)
                err = tempErr;
        }

        tempErr = PMRelease(printSession);    
        if(err == noErr)
            err = tempErr;
    }
    DoErrorAlert(err, kMySaveAsPDFErrorFormatStrKey);
    return err;
}
