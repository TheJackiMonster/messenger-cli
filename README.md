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

Then you can simply use the provided Makefile as follows:

 - `make` to just compile everything with default parameters
 - `make clean` to cleanup build files in case you want to recompile
 - `make debug` to compile everything with debug parameters
 - `make release` to compile everything with build optimizations enabled
 - `make install` to install the compiled files (you might need sudo permissions to install)

## Contribution

If you want to contribute to this project as well, the following options are available:

 - Contribute directly to the [source code](https://git.gnunet.org/messenger-cli.git/) with patches to fix issues, implement new features or improve the usability.
 - Open issues in the [bug tracker](https://bugs.gnunet.org/bug_report_page.php) to report bugs, issues or missing features.
 - Contact the authors of the software if you need any help to contribute (testing is always an option).

The list of all previous authors can be viewed in the provided [file](AUTHORS).

