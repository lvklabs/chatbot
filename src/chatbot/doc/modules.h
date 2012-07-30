/**

\defgroup Lvk LVK Chatbot

LVK Chatbot is made-up with following modules: \ref FE, \ref BE \ref Nlp, \ref CA, \ref Cmn


\defgroup FE Front-end

\brief The Front-end module contains all the windows, dialogs and widgets used to display the
graphical user interface (GUI) to the user. The front-end does process any request, instead it
forwards all requests to the \ref BE module.


\defgroup BE Back-end

\brief  The Back-end module provides all the functionality required by the UI such as, save/open
files, import/export rules, connect to chat server, chat conversation history, etc. To provide such
functionality Back-end module depends on
\ref CA and \ref Nlp. 


\defgroup Nlp NLP Engine

\brief The NLP engine module provides a Natural Language Processing engine that is used by chatbots
to reply users.


\defgroup CA Chat adapters

\brief The Chat Adapters module provides connection to chat servers such as GTalk and Facebook Chat.


\defgroup Cmn Common

\brief The Common module provides classes, macros, strings and other declarations that are common
to all modules.

*/
