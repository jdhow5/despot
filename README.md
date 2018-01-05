# DESpot
In this repository I have included the source code for DESpot v.1.7.0 in DESpot_v1_7.tar.bz2.  I have also included the individual files that I contributed to DESpot during the development of the regression test suite.  The regression test suite was my Master of Engeering in Software Engineering project at McMaster University.

DESpot is a discrete-event system (DES) software research tool.  The regression testing suite allows future developers to ensure their modifications to DESpot's system checking algorithms provide the correct results and do not introduce new bugs.  It also allows users to quickly check chosen system properties while creating the DES in DESpot's "Project Editor".

The regression test suite was developed with C++ and Qt 5.8.  I used Qt Creator as the IDE and Qt Designer to create the user interface forms.

For more information about DESpot and its use, development, and a download of the latest version please visit [DESpot's webpage](http://www.cas.mcmaster.ca/~leduc/DESpot.html)

# Installation
DESpot requires the BuDDy BDD libraries which can be downloaded from [here](https://sourceforge.net/projects/buddy/)

1. Download the tar file of DESpot and the BuDDy BDD libraries
2. Untar the DESpot and BuDDy source code
3. Configure and compile the BuDDy libraries by running as root in the BuDDy directory:
  * ./configure --libdir=/usr/lib --includedir=/usr/include
  * make
  * make install
4. Change to the DESpot directory and run:
  * qmake-qt4 DESpot.pro
  * make release
5. To run DESpot type:
  * cd release
  * ./DESpot

For more detailed instructions, please visit [DESpot's installation instructions](http://www.cas.mcmaster.ca/~leduc/software/DESpot/DESpotExternBuildGuide.txt).

### Note
The source code included has been tested on Ubuntu 16.04 LTS but this is not the final release of DESpot v.1.7.0.  The version of DESpot found on its website will not be updated to v.1.7.0 (v.1.6.0 does not include the regression test suite) until it has been successfully compiled and tested on Windows and other Linux distros.

# Author
Jacob Howard 2017
