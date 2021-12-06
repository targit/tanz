#include <chrono>
#include <gtest/gtest.h>
#include <tanz/optional-queued-promise.h++>
#include <string>
#include <thread>

TEST( optional_queued_promise, one_run )
{
    using p_t = tz::optional_threaded_t< std::string >;
    p_t oqp;

    EXPECT_EQ( oqp.state(), p_t::UNSET );
    EXPECT_EQ( oqp.update(), 0 );

    oqp =
        []()
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 5 ));
            return std::string("abc");
        };
    EXPECT_EQ( oqp.state(), p_t::PENDING );
    EXPECT_EQ( oqp.update(), 0 );
    // EXPECT_EQ( oqp.state(), p_t::PENDING );
    (*(oqp.pending)).wait();
    EXPECT_EQ( oqp.update(), 1 );
    EXPECT_EQ( oqp.state(), p_t::SET );
    EXPECT_EQ( *oqp, "abc");

}

TEST( optional_queued_promise, queued_run )
{
    using p_t = tz::optional_threaded_t< std::string >;
    p_t oqp;

    EXPECT_EQ( oqp.state(), p_t::UNSET );
    EXPECT_EQ( oqp.update(), 0 );

    oqp =
        []()
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 6 ));
            return std::string("abc");
        };
    EXPECT_EQ( oqp.state(), p_t::PENDING );
    EXPECT_FALSE( bool(oqp)); // calls update() implicitly */
    oqp =
        []()
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 3 ));
            return std::string("xyz");
        };
    oqp =
        []()
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 6 ));
            return std::optional< std::string > { "xyz" };
        };
    oqp.update();
    oqp = {};
    oqp =
        []()
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 6 ));
            return std::string("xyz");
        };
    oqp =
        std::move(
        []()
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ));
            return std::optional< std::string >{ "def" };
        });
    oqp.update();
    (*(oqp.pending)).wait();
    oqp.update();
    (*(oqp.pending)).wait();
    oqp.update();
    EXPECT_EQ( oqp.state(), p_t::SET );
    EXPECT_EQ( *oqp, "def" );
}
