#include <boost/test/unit_test.hpp>
#include <data/GameTexts.hpp>
#include <engine/ScreenText.hpp>
#include <loaders/LoaderGXT.hpp>
#include "test_globals.hpp"

#define T(x) GameStringUtil::fromString(x)

BOOST_AUTO_TEST_SUITE(TextTests)

#if RW_TEST_WITH_DATA
BOOST_AUTO_TEST_CASE(load_test) {
    {
        auto d = Global::get().e->data->index.openFilePath("text/english.gxt");

        GameTexts texts;

        LoaderGXT loader;

        loader.load(texts, d);

        BOOST_CHECK_EQUAL(texts.text("1008"), T("BUSTED"));
    }
}

BOOST_AUTO_TEST_CASE(big_test) {
    // Check that makeBig creates a text in the right place
    {
        auto big =
            ScreenTextEntry::makeBig("TEST_1", T("Test String"), 1, 5000);

        BOOST_CHECK_EQUAL("TEST_1", big.id);
        BOOST_CHECK_EQUAL(T("Test String"), big.text);
        BOOST_CHECK_EQUAL(5000, big.durationMS);
        BOOST_CHECK_EQUAL(0, big.displayedMS);
        BOOST_CHECK_EQUAL(1, big.alignment);
        BOOST_CHECK_EQUAL(50, big.size);
    }
    {
        auto big =
            ScreenTextEntry::makeBig("TEST_1", T("Test String"), 2, 5000);

        BOOST_CHECK_EQUAL(T("Test String"), big.text);
        BOOST_CHECK_EQUAL(5000, big.durationMS);
        BOOST_CHECK_EQUAL(0, big.displayedMS);
        BOOST_CHECK_EQUAL(2, big.alignment);
        BOOST_CHECK_EQUAL(30, big.size);
    }
}

BOOST_AUTO_TEST_CASE(help_test) {
    auto help = ScreenTextEntry::makeHelp("TEST_1", T("Test Help"));

    BOOST_CHECK_EQUAL(T("Test Help"), help.text);
    BOOST_CHECK_EQUAL(5000, help.durationMS);
    BOOST_CHECK_EQUAL(0, help.displayedMS);
    BOOST_CHECK_EQUAL(18, help.size);
}

BOOST_AUTO_TEST_CASE(queue_test) {
    // Check that creating a test puts it on the right queue.

    ScreenText st;

    st.addText<ScreenTextType::Big>(
        ScreenTextEntry::makeBig("TEST_1", T("Test String"), 2, 5000));
    st.addText<ScreenTextType::HighPriority>(
        ScreenTextEntry::makeHighPriority("TEST_1", T("Test String"), 5000));

    BOOST_REQUIRE(st.getText<ScreenTextType::Big>().size() == 1);
    BOOST_CHECK_EQUAL(T("Test String"),
                      st.getText<ScreenTextType::Big>()[0].text);
    BOOST_CHECK_EQUAL(5000, st.getText<ScreenTextType::Big>()[0].durationMS);
    BOOST_CHECK_EQUAL(0, st.getText<ScreenTextType::Big>()[0].displayedMS);

    BOOST_CHECK_EQUAL(1, st.getText<ScreenTextType::HighPriority>().size());

    st.tick(6.f);

    BOOST_CHECK_EQUAL(0, st.getText<ScreenTextType::Big>().size());
    BOOST_CHECK_EQUAL(0, st.getText<ScreenTextType::HighPriority>().size());
}

BOOST_AUTO_TEST_CASE(clear_test) {
    ScreenText st;

    st.addText<ScreenTextType::Big>(
        ScreenTextEntry::makeBig("TEST_1", T("Test String"), 2, 5000));

    BOOST_CHECK_EQUAL(1, st.getText<ScreenTextType::Big>().size());

    st.clear<ScreenTextType::Big>();

    BOOST_CHECK_EQUAL(0, st.getText<ScreenTextType::Big>().size());
}

BOOST_AUTO_TEST_CASE(format_test) {
    // Test formating of string codes into strings.
    const auto codeStr1 = T("Hello ~1~ world");
    const auto codeStr2 = T("~1~Hello ~1~ world~1~");
    const auto codeStr3 = T("Hello world~1~");

    auto f1 = ScreenText::format(codeStr1, T("r"));
    BOOST_CHECK_EQUAL(f1, T("Hello r world"));

    auto f2 = ScreenText::format(codeStr2, T("k"), T("h"));
    BOOST_CHECK_EQUAL(f2, T("kHello h world~1~"));

    auto f3 = ScreenText::format(codeStr3, T("x"));
    BOOST_CHECK_EQUAL(f3, T("Hello worldx"));

    auto f4 = ScreenText::format(codeStr3, T("x"), T("k"));
    BOOST_CHECK_EQUAL(f4, T("Hello worldx"));
}

BOOST_AUTO_TEST_CASE(format_remove) {
    // Test removing an identified string from the list
    ScreenText st;

    st.addText<ScreenTextType::Big>(
        ScreenTextEntry::makeBig("TEST_2", T("Test String"), 2, 5000));

    st.addText<ScreenTextType::Big>(
        ScreenTextEntry::makeBig("TEST_1", T("Test String"), 2, 5000));

    st.addText<ScreenTextType::Big>(
        ScreenTextEntry::makeBig("TEST_1", T("Test String"), 2, 5000));

    BOOST_CHECK_EQUAL(3, st.getText<ScreenTextType::Big>().size());

    st.remove<ScreenTextType::Big>("TEST_1");

    BOOST_CHECK_EQUAL(1, st.getText<ScreenTextType::Big>().size());
}
#endif

BOOST_AUTO_TEST_SUITE_END()
