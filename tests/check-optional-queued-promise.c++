#include <chrono>
#include <gtest/gtest.h>
#include <tanz/optional_queued_promise.h++>
#include <string>
#include <thread>

TEST( optional_queued_promise, one_run )
{
    using p_t = tz::optional_queued_promise_t< std::string >;
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
    using p_t = tz::optional_queued_promise_t< std::string >;
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
    oqp.update();
    EXPECT_NE( oqp.state(), p_t::SET );
    oqp.update();
    EXPECT_NE( oqp.state(), p_t::SET );
    oqp =
        []()
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 3 ));
            return std::string("xyz");
        };
    oqp =
        std::move(
        []()
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ));
            return std::string("def");
        });
    oqp.update();
    (*(oqp.pending)).wait();
    oqp.update();
    (*(oqp.pending)).wait();
    oqp.update();
    EXPECT_EQ( oqp.state(), p_t::SET );
    EXPECT_EQ( *oqp, "def" );
}