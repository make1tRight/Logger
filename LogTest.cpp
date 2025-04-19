#include "Logger.h"

int main() {
    try {
        Logger logger;

        logger.addsink(std::make_shared<FileSink>());
        logger.addsink(std::make_shared<ConsoleSink>());

        logger.log("Start application.");

        int user_id = 42;
        std::string action = "Login";
        double duration = 3.5;
        std::string world = "World";

        logger.log("User {} performed {} in {} seconds.",
             user_id, action, world);
        logger.log("Hello {}", world);
        // logger.log("This is a message without placeholder.");
        logger.log("Multiple placeholders {}, {}, {}.", 1, 2, 3);

        for (int i = 5000000; i >= 0; --i) {
            logger.log("This is a message without placeholder.");
        }
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}