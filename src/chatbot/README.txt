LVK CHATBOT README
------------------

 -- To compile:

  $ qmake && make 

  WARNING: Mac version is not fully supported. 

  NOTE: Requires: Qt SDK   >= 4.7.0 
                  QXmpp    >= 0.3.0 (*1)
 
        Optional: 
                  Freeling >= 2.2.2 (*2)
                  zlib     >= 1.2.7 (*3)

  (*1) Precompiled static library of QXmpp is already provided for 
  linux32, win32 and mac64. Other OSes will require to compile QXmpp 
  manually. Full sources are available at src/third-party/QXmpp
  
  (*2) Precompiled static library is already provided for linux32 and
  win32. Other OSes will require to compile Freeling manually. Full
  sources are available at src/third-party/Freeling. 
  To disable Freeling remove 'CONFIG += freeling' in chatbot.pro

  (*3) Precompiled static library is already provided for linux32 and
  win32. Other OSes might require to compile Zlib manually. Full
  sources are available at src/third-party/zlib. Zlib is required
  to send GELF messages to a Graylog server. To disable GELF statistic
  support remove 'CONFIG += gelf_stats' in chatbot.pro



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
 $ ./run-test-suite.sh [ -a | -u | -e ]

  NOTE: Same dependencies to compile the main project


