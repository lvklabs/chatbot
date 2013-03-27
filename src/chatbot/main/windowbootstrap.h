/*
 * Copyright (C) 2012 Andres Pagliano, Gabriel Miretti, Gonzalo Buteler,
 * Nestor Bustamante, Pablo Perez de Angelis
 *
 * This file is part of LVK Chatbot.
 *
 * LVK Chatbot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LVK Chatbot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LVK Chatbot.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef WINDOWBOOTSTRAP_H
#define WINDOWBOOTSTRAP_H

#include "front-end/mainwindow.h"

#ifdef ENABLE_WELCOME_WINDOW
# include "front-end/welcomewidget.h"
#endif

/**
 * \brief The WindowBootstrap class is in charge of bootstraping the application window.
 *
 * The WindowBootstrap is a helper class used by the application entry point, that is main().
 * The class must be constructed with the application command line arguments. If the
 * first parameter does not starts with '-' then a Chatbot file is assumed and the file is
 * opened. If no Chatbot file is found then the "welcome" window is displayed.
 */
class WindowBootstrap
{
public:

    /**
     * Constructs a WindowBootstrap class with command line arguments \a argc and \a argv.
     * According the parameters the proper window is constructed and displayed. If the
     * first parameter does not starts with '-' then a Chatbot file is assumed and the file is
     * opened. If no Chatbot file is found then the "welcome" window is displayed.
     */
    WindowBootstrap(int argc, char *argv[])
        : m_w(0)
    {
        QString filename;

        if (argc > 1 && argv[1][0] != '-') {
            m_w = new MainWindow();
            filename = argv[1];
        } else {
            m_w = new WelcomeWindow();
        }

        m_w->show();

        if (!filename.isEmpty()) {
            dynamic_cast<MainWindow*>(m_w)->openFile(filename);
        }
    }

    /**
     * Destroys the object along with the created window.
     */
    ~WindowBootstrap()
    {
        delete m_w;
    }

private:

#ifdef ENABLE_WELCOME_WINDOW
    typedef Lvk::FE::WelcomeWidget WelcomeWindow;
    typedef Lvk::FE::MainWindow    MainWindow;
#else
    typedef Lvk::FE::MainWindow    WelcomeWindow;
    typedef Lvk::FE::MainWindow    MainWindow;
#endif

    QWidget *m_w;
};

#endif // WINDOWBOOTSTRAP_H
