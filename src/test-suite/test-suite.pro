TEMPLATE = subdirs

CONFIG += ordered

unit_tests {
    SUBDIRS += \
        default-sanitizer-unit-test \
        aiml-engine-unit-test  \
        hybrid-engine-unit-test \
        csv-document-unit-test \
        conversation-rw-unit-test \
        secure-stats-file-unit-test \
        stats-manager-test \
        cipher-unit-test
}

end_to_end_tests {
    SUBDIRS += \
        end-to-end-test \
}


