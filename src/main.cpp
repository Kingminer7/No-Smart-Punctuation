#include <Geode/Geode.hpp>
#include <Geode/modify/CCIMEDispatcher.hpp>

using namespace geode::prelude;

class $modify(CCIMEDispatcherNoSmartPunctuation, CCIMEDispatcher) {
    static inline int sizeu8(const char* s, int len) {
        int count = 0;
        for (int i = 0; i < len; ++i) {
            if ((s[i] & 0xC0) != 0x80) count++;
        }
        return count;
    }

    void dispatchInsertText(const char* text, int len, cocos2d::enumKeyCodes code) {
        if (sizeu8(text, len) > 1) {
            std::string out(text, len);

            string::replaceIP(out, "“", "\"");
            string::replaceIP(out, "”", "\"");
            string::replaceIP(out, "‘", "'");
            string::replaceIP(out, "’", "'");
            string::replaceIP(out, "—", "--");
            string::replaceIP(out, "–", "-");
            string::replaceIP(out, "…", "...");

            CCIMEDispatcher::dispatchInsertText(out.c_str(), out.size(), code);
            return;
        }

        // separate functionality because sending multiple chars is handled differently for some reason (it replaces the whole text)
        // its prolly some weird func in CCTextInputNode ngl

        if (!memcmp(text, "—", 3)) {
            CCIMEDispatcher::dispatchInsertText("-", 1, code);
            CCIMEDispatcher::dispatchInsertText("-", 1, cocos2d::enumKeyCodes::KEY_Unknown);
            return;
        }

        if (!memcmp(text, "…", 3)) {
            CCIMEDispatcher::dispatchInsertText(".", 1, code);
            CCIMEDispatcher::dispatchInsertText(".", 1, cocos2d::enumKeyCodes::KEY_Unknown);
            CCIMEDispatcher::dispatchInsertText(".", 1, cocos2d::enumKeyCodes::KEY_Unknown);
            return;
        }

        if (!memcmp(text, "“", 3) || !memcmp(text, "”", 3))
            return CCIMEDispatcher::dispatchInsertText("\"", 1, code);

        if (!memcmp(text, "‘", 3) || !memcmp(text, "’", 3))
            return CCIMEDispatcher::dispatchInsertText("'", 1, code);

        if (!memcmp(text, "–", 3))
            return CCIMEDispatcher::dispatchInsertText("-", 1, code);

        CCIMEDispatcher::dispatchInsertText(text, len, code);
    }
};