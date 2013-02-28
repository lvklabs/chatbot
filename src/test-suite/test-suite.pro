TEMPLATE = subdirs

CONFIG += ordered

unit_tests {
    SUBDIRS += \
        json-unit-test \
        default-sanitizer-unit-test \
        cb2-engine-unit-test \
        csv-document-unit-test \
        conversation-rw-unit-test \
        secure-stats-file-unit-test \
        cipher-unit-test \
        updater-unit-test \
        script-manager-unit-test
}

end_to_end_tests {
    SUBDIRS += \
        stats-manager-test \
        user-auth-test \
        cb2-engine-full-test
}

OTHER_FILES += run-test-suite.sh

