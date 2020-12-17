# Setup
1. Clone the whole [OpenAMF repository](https://github.com/amfdev/AMF) or update your downloaded repository using *git pull*.
2. Download all submodules, using *git submodule update --init*

# Build instructions
1. Generate a suitable project configuration (depending on your OS: static/shared, vs2017/2019, cmake/xcode), using one of the "generate-*.(bat|sh)". You don't need to pass any parameters to the script.
2. Build generated project, using your IDE or one of the "build-*.(bat|sh)" scripts.
3. Run the generated executable file (name and folder may vary depending on the OS). Read about the available GTest flags [here](https://github.com/google/googletest/blob/master/googletest/docs/advanced.md#running-test-programs-advanced-options)
