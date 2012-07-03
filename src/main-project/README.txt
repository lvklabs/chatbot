LVK CHATBOT README
------------------

 -- To compile:

  $ qmake && make 

  NOTE: Requires Qt SDK >= 4.7.0 
                 QXmpp  >= 0.3.0 (*)

  (*) QXmpp full sources are already provided in src/third-party/QXmpp
      For win32 and linux32 precompiled static libraries are already 
      provided in src/third-party/QXmpp/lib


 -- To Run:

  $ ./chatbot



 -- To generate Doxygen documentation:

  $ bin/udpate-doc.sh

  NOTE: Requires doxygen package and it is also recommended 
        the dot package.



 -- To update strings localized in Spanish:

  $ bin/update-strings.sh

  NOTE: Requires Qt SDK >= 4.7.0



 -- How to compile and run tests?

 $ cd ../test-suite
 $ ./run-test-suite.sh

  NOTE: Same dependencies to compile the main project


