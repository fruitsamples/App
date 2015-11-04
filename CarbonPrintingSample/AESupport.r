/*
	AESupport.r
	Coypright © 2004-7 Apple Inc. All Rights Reserved

	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Inc.
				("Apple") in consideration of your agreement to the following terms, and your
				use, installation, modification or redistribution of this Apple software
				constitutes acceptance of these terms.  If you do not agree with these terms,
				please do not use, install, modify or redistribute this Apple software.

				In consideration of your agreement to abide by the following terms, and subject
				to these terms, Apple grants you a personal, non-exclusive license, under Apple’s
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
#include <Carbon/Carbon.r>

#define	PRE103	1

#if PRE103
#define	kPMPrintSettingsAEType			'pset'
#define kPMShowPrintDialogAEType		'pdlg'
#define kPMPrinterAEType                'trpr'

#define kPMCopiesAEProp					"copies"
#define kPMCopiesAEKey					'lwcp'
#define kPMCopieAEType					typeSInt32

#define kPMCollateAEProp				"collating"
#define kPMCollateAEKey					'lwcl'
#define kPMCollateAEType				typeBoolean

#define kPMFirstPageAEProp				"starting page"
#define kPMFirstPageAEKey				'lwfp'
#define kPMFirstPageAEType				typeSInt32

#define kPMLastPageAEProp				"ending page"
#define kPMLastPageAEKey				'lwlp'
#define kPMLastPageAEType				typeSInt32

#define kPMLayoutAcrossAEProp				"pages across"
#define kPMLayoutAcrossAEKey				'lwla'
#define kPMLayoutAcrossAEType				typeSInt32

#define kPMLayoutDownAEProp				"pages down"
#define kPMLayoutDownAEKey				'lwld'
#define kPMLayoutDownAEType				typeSInt32

#define kPMErrorHandlingAEProp				"error handling"
#define kPMErrorHandlingAEKey				'lweh'
#define kPMErrorHandlingAEType				typeEnumerated

#define kPMPrintTimeAEProp				"requested print time"
#define kPMPrintTimeAEKey				'lwqt'
#define kPMPrintTimeAEType				cLongDateTime

#define kPMFeatureAEProp				"printer features"
#define kPMFeatureAEKey					'lwpf'
#define kPMFeatureAEType				typeAEList

#define	kPMFaxNumberAEProp				"fax number"
#define kPMFaxNumberAEKey				'faxn'
#define kPMFaxNumberAEType				typeChar

#define kPMTargetPrinterAEProp			"target printer"
#define kPMTargetPrinterAEKey			'trpr'
#define kPMTargetPrinterAEType			typeChar

/*** Enumerations ***/

/* For kPMErrorHandlingAEType */
#define kPMErrorHandlingStandardEnum		'lwst'
#define kPMErrorHandlingDetailedEnum		'lwdt'
#else
#include <ApplicationServices/ApplicationServices.r>
#endif


data 'carb' (0) {
};

resource kAETerminologyExtension (0) {
	0x1,
	0x0,
	english,
	roman,
	{	/* array Suites: 2 elements */
		/* [1] */
		"Standard Suite",
		"Common terms for most applications",
		'CoRe',
		1,
		1,
		{	/* array Events: 4 elements */
			/* [1] */
			"open",
			"Open the specified object(s)",
			kCoreEventClass,
			kAEOpenDocuments,
			noReply,
			"",
			replyOptional,
			singleItem,
			notEnumerated,
			notTightBindingFunction,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			verbEvent,
			reserved,
			reserved,
			reserved,
			cObjectSpecifier,
			"Objects to open. Can be a list of files or an object specifier.",
			directParamRequired,
			singleItem,
			notEnumerated,
			changesState,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			{	/* array OtherParams: 0 elements */
			},
			/* [2] */
			"print",
			"Print the specified object(s)",
			kCoreEventClass,
			kAEPrintDocuments,
			noReply,
			"",
			replyOptional,
			singleItem,
			notEnumerated,
			notTightBindingFunction,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			verbEvent,
			reserved,
			reserved,
			reserved,
			cObjectSpecifier,
			"Objects to print. Can be a list of files or an object specifier.",
			directParamRequired,
			listOfItems,
			notEnumerated,
			doesntChangeState,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			{	/* array OtherParams: 2 elements */
				/* [1] */
				"with properties",
				keyAEPropData,
				kPMPrintSettingsAEType,
				"the print settings",
				optional,
				singleItem,
				notEnumerated,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				prepositionParam,
				notFeminine,
				notMasculine,
				singular,
				/* [2] */
				"print dialog",
				kPMShowPrintDialogAEType,
				typeBoolean,
				"Should the application show the print dialog?",
				optional,
				singleItem,
				notEnumerated,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				prepositionParam,
				notFeminine,
				notMasculine,
				singular
			},
			/* [3] */
			"run",
			"Sent to an application when it is double-clicked",
			kCoreEventClass,
			kAEOpenApplication,
			noReply,
			"",
			replyOptional,
			singleItem,
			notEnumerated,
			notTightBindingFunction,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			verbEvent,
			reserved,
			reserved,
			reserved,
			noParams,
			"No direct parameter required",
			directParamOptional,
			singleItem,
			notEnumerated,
			changesState,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			{	/* array OtherParams: 0 elements */
			},
			/* [4] */
			"quit",
			"Quit application",
			kCoreEventClass,
			kAEQuitApplication,
			noReply,
			"",
			replyOptional,
			singleItem,
			notEnumerated,
			notTightBindingFunction,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			verbEvent,
			reserved,
			reserved,
			reserved,
			noParams,
			"No direct parameter required",
			directParamOptional,
			singleItem,
			notEnumerated,
			changesState,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			reserved,
			{	/* array OtherParams: 0 elements */
			}
		},
		{	/* array Classes: 1 elements */
			/* [1] */
			"print settings",
			kPMPrintSettingsAEType,
			"",
				{
				/* [1] */
				kPMCopiesAEProp,
				kPMCopiesAEKey,
				kPMCopieAEType,
				"the number of copies of a document to be printed ",
				reserved,
				singleItem,
				notEnumerated,
				readOnly,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				noApostrophe,
				notFeminine,
				notMasculine,
				singular,

				/* [2] */
				kPMCollateAEProp,
				kPMCollateAEKey,
				kPMCollateAEType,
				"Should printed copies be collated?",
				reserved,
				singleItem,
				notEnumerated,
				readOnly,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				noApostrophe,
				notFeminine,
				notMasculine,
				singular,

				/* [6] */
				kPMFirstPageAEProp,
				kPMFirstPageAEKey,
				kPMFirstPageAEType,
				"the first page of the document to be printed",
				reserved,
				singleItem,
				notEnumerated,
				readOnly,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				noApostrophe,
				notFeminine,
				notMasculine,
				singular,

				/* [7] */
				kPMLastPageAEProp,
				kPMLastPageAEKey,
				kPMLastPageAEType,
				"the last page of the document to be printed",
				reserved,
				singleItem,
				notEnumerated,
				readOnly,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				noApostrophe,
				notFeminine,
				notMasculine,
				singular,

				/* [8] */
				kPMLayoutAcrossAEProp,
				kPMLayoutAcrossAEKey,
				kPMLayoutAcrossAEType,
				"number of logical pages laid across a physical page",
				reserved,
				singleItem,
				notEnumerated,
				readOnly,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				noApostrophe,
				notFeminine,
				notMasculine,
				singular,

				/* [9] */
				kPMLayoutDownAEProp,
				kPMLayoutDownAEKey,
				kPMLayoutDownAEType,
				"number of logical pages laid out down a physical page",
				reserved,
				singleItem,
				notEnumerated,
				readOnly,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				noApostrophe,
				notFeminine,
				notMasculine,
				singular,

				/* [10] */
				kPMPrintTimeAEProp,
				kPMPrintTimeAEKey,
				kPMPrintTimeAEType,
				"the time at which the desktop printer should print the document...",
				reserved,
				singleItem,
				notEnumerated,
				readOnly,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				noApostrophe,
				notFeminine,
				notMasculine,
				singular,

				/* [13] */
				kPMErrorHandlingAEProp,
				kPMErrorHandlingAEKey,
				kPMErrorHandlingAEType,
				"how errors are handled",
				reserved,
				singleItem,
				notEnumerated,
				readOnly,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				noApostrophe,
				notFeminine,
				notMasculine,
				singular,

				/* [14] */
				kPMFaxNumberAEProp,
				kPMFaxNumberAEKey,
				kPMFaxNumberAEType,
				"for fax number",
				reserved,
				singleItem,
				notEnumerated,
				readOnly,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				noApostrophe,
				notFeminine,
				notMasculine,
				singular,

				/* [15] */
				kPMTargetPrinterAEProp,
				kPMTargetPrinterAEKey,
				kPMTargetPrinterAEType,
				"for target printer",
				reserved,
				singleItem,
				notEnumerated,
				readOnly,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				reserved,
				noApostrophe,
				notFeminine,
				notMasculine,
				singular
			},
			{	/* array Elements: 0 elements */
			}
		},
		{	/* array ComparisonOps: 0 elements */
		},
		{	/* array Enumerations: 2 elements */
			kPMErrorHandlingAEType,
			{	/* array Enumerators: 3 elements */
				/* [1] */
				"standard",
				kPMErrorHandlingStandardEnum,
				"Standard PostScript error handling  ",
				/* [2] */
				"detailed",
				kPMErrorHandlingDetailedEnum,
				"print a detailed report of PostScript errors"
			}
		},
		/* [2] */
		"Required Suite",
		"Terms that every application should support",
		kAERequiredSuite,
		1,
		1,
		{	/* array Events: 0 elements */
		},
		{	/* array Classes: 0 elements */
		},
		{	/* array ComparisonOps: 0 elements */
		},
		{	/* array Enumerations: 0 elements */
		},
	}
};
