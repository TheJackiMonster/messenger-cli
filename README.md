# messenger-cli

A CLI for the Messenger service of GNUnet.

```
                            
    o                   o   
  ooo                   oo  
     ooooo        ooooo     
         oo      ooo        
          oo    ooo         
           ooooooo          
           oooooo           
            oooo            
                            

```

## Build & Installation

The following dependencies are required and need to be installed to build the application:

 - [gnunet](https://git.gnunet.org/gnunet.git/): For using general GNUnet datatypes
 - [libgnunetchat](https://git.gnunet.org/libgnunetchat.git/): For chatting via GNUnet messenger
 - [ncurses](https://www.gnu.org/software/ncurses/): For the general UI visualization

Then you can simply use [Autotools](https://www.gnu.org/software/automake/) as follows:
```
./bootstrap        # Generate the configure script
./configure        # Configure the Makefiles for your system
make               # Build the application using the Makefiles
sudo make install  # Install the application
```

Here is a list of some useful build targets in the Makefile:

 - `make` to just compile everything with default parameters
 - `make clean` to cleanup build files in case you want to recompile
 - `make install` to install the compiled files (you might need sudo permissions to install)

If you want to change the installation location, use the `--prefix=` parameter in the `configure` script. Also you can enable debugging builds by adding `--enable-debug` as parameter when running the `configure` script.

## Contribution

If you want to contribute to this project as well, the following options are available:

 - Contribute directly to the [source code](https://git.gnunet.org/messenger-cli.git/) with patches to fix issues, implement new features or improve the usability.
 - Open issues in the [bug tracker](https://bugs.gnunet.org/bug_report_page.php) to report bugs, issues or missing features.
 - Contact the authors of the software if you need any help to contribute (testing is always an option).

The list of all previous authors can be viewed in the provided [file](AUTHORS).

