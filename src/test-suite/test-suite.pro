TEMPLATE = subdirs

CONFIG += ordered

unit_tests {
    SUBDIRS += \
        json-unit-test \
        default-sanitizer-unit-test \
        aiml-engine-unit-test  \
        hybrid-engine-unit-test \
        csv-document-unit-test \
        conversation-rw-unit-test \
        secure-stats-file-unit-test \
        stats-manager-test \
        cipher-unit-test \
        updater-unit-test 
}

end_to_end_tests {
    SUBDIRS += \
        main-window-test \
        user-auth-test \
}


