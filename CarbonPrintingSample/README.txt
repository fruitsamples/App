This is a simple document based application that can use sheets during 
printing.

Extra features include:
a) Print One Copy handling (printing without the Print dialog)
b) Demonstration of Save As PDF and Save as PostScript functionality 
without using the printing dialogs.
c) Ability to choose whether or not to use sheets when printing.
d) Ability to choose whether or not to get status narration during
save as PDF or PostScript.
e) Using a custom print panel using Cocoa.

# to build and install debug 2 way fat
xcodebuild -configuration Debug RC_ARCHS="ppc i386"

# to build and install release 2 way fat
sudo xcodebuild -configuration Release RC_ARCHS="ppc i386"

NOTES on the Cocoa custom print panel:

The ability to create custom print panels easily by creating a Cocoa bundle 
(even in a Carbon application). The ability was introduced in Mac OS X version 10.4. 
This sample code demonstrates this capability with a simple panel.

In Mac OS X version 10.5 and later, it is possible to enable garbage collection in a
Cocoa application. All code loaded by a garbage collected enabled application must also 
have garbage collection enabled. This includes printing bundles such as custom print panels.
In the case of the custom print panel demonstrated in this example, it is built WITHOUT
garbage collection enabled. This is done because the bundle is being used by a Carbon application
and Objective-C garbage collection is meaningless in that context. IF you are adapting 
this custom print panel for use with a Cocoa application that is garbage collected, you
should build the Cocoa bundle with garbage collection enabled and properly implement a
finalize method as needed.

NOTE: Cocoa application developers are urged to use the NSViewController introduced in Mac OS X
version 10.5 instead of writing Cocoa bundles such as the one used in this sample.
 