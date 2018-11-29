Note: The AIMMSCOM functionality has been deprecated in favor of the AIMMS API and AIMMS PRO API!

This project illustrates how AIMMS can be used as an optimization component from within an external program, using Microsoft COM technology. In this example data is transfered back and forth to an AIMMS model. The external program will also call a procedure within the AIMMS model. The AIMMS model is a simple transport model and contains all relevant declarations and procedures. The example illustrates both a C program as well as a python script. 

In the model formulation of this example project no input data is read at all. Instead, all input data is supplied from within the external program through the AIMMSCOM interface.

To run the external program:
- make sure that you have installed the AIMMS Launcher; this will automatically register the AIMMS COM interface on a per-user basis. It allows you to use the AIMMS COM interface for any installation-free AIMMS version installed on your machine.
- run the program RunAIMMS.exe

The results of optimizing the transport model are displayed in the file RunAIMMS.out.

The RunAIMMS.exe executable can be made via Microsoft Visual Studio 2013. To create the RunAIMMS.exe program yourself:
- make sure you set the proper environment variables for your compiler and target platform first (all compilers provide batch files to do this)
- run 'nmake' in the 'Source' subfolder
- copy the created 'RunAIMMS.exe' program to the folder where the AIMMS project (RunAIMMS.aimms) is located (i.e. 'Calling AIMMSCOM')

To run the python script RunAIMMS.py:
- make sure that you have installed the AIMMS Launcher; this will automatically register the AIMMS COM interface on a per-user basis. It allows you to use the AIMMS COM interface for any installation-free AIMMS version installed on your machine.
- edit the script to let projectName point to the actual full path of RunAIMMS.aimms file
- run python RunAIMMS.py
