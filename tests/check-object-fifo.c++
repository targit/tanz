#include <gtest/gtest.h>
#include <tanz/object-fifo.h++>

TEST( object_fifo, io )
{
    using fifo_t = tz::object_fifo< double >;

    fifo_t fifo( 2 );

    EXPECT_FALSE( fifo.closed() );
    EXPECT_TRUE( fifo.empty());
    EXPECT_FALSE( fifo.full() );

    fifo.write_blockingly( 1.0 );
    EXPECT_FALSE( fifo.closed() );
    EXPECT_FALSE( fifo.empty() );
    EXPECT_FALSE( fifo.full() );

    fifo.read_blockingly();
    EXPECT_FALSE( fifo.closed() );
    EXPECT_TRUE( fifo.empty() );
    EXPECT_FALSE( fifo.full() );

    fifo.write_blockingly( 1.0 );
    fifo.write_blockingly( 2.0 );
    EXPECT_FALSE( fifo.closed() );
    EXPECT_FALSE( fifo.empty() );
    EXPECT_TRUE( fifo.full() );

    fifo.write_overwrite( 3.0 );
    EXPECT_FALSE( fifo.closed() );
    EXPECT_FALSE( fifo.empty() );
    EXPECT_TRUE( fifo.full() );

    {
        auto pop0 = fifo.read_blockingly();
        EXPECT_EQ( pop0, 1.0 );
        EXPECT_FALSE( fifo.closed() );
        EXPECT_FALSE( fifo.empty() );
        EXPECT_FALSE( fifo.full() );
    }
    {
        auto pop1 = fifo.read_blockingly();
        EXPECT_EQ( pop1, 3.0 );
        EXPECT_FALSE( fifo.closed() );
        EXPECT_TRUE( fifo.empty() );
        EXPECT_FALSE( fifo.full() );
    }
}

TEST( object_fifo, unlimited )
{
    using fifo_t = tz::object_fifo< double >;

    fifo_t fifo;

    EXPECT_FALSE( fifo.closed() );
    EXPECT_TRUE( fifo.empty());
    EXPECT_FALSE( fifo.full() );

    fifo.write_blockingly( 1.0 );
    EXPECT_FALSE( fifo.closed() );
    EXPECT_FALSE( fifo.empty() );
    EXPECT_FALSE( fifo.full() );

    fifo.read_blockingly();
    EXPECT_FALSE( fifo.closed() );
    EXPECT_TRUE( fifo.empty() );
    EXPECT_FALSE( fifo.full() );

    fifo.write_blockingly( 1.0 );
    fifo.write_blockingly( 2.0 );
    EXPECT_FALSE( fifo.closed() );
    EXPECT_FALSE( fifo.empty() );
    EXPECT_FALSE( fifo.full() );

    fifo.write_overwrite( 3.0 );
    EXPECT_FALSE( fifo.closed() );
    EXPECT_FALSE( fifo.empty() );
    EXPECT_FALSE( fifo.full() );

    {
        auto pop0 = fifo.read_blockingly();
        EXPECT_EQ( pop0, 1.0 );
        EXPECT_FALSE( fifo.closed() );
        EXPECT_FALSE( fifo.empty() );
        EXPECT_FALSE( fifo.full() );
    }
    {
        auto pop1 = fifo.read_blockingly();
        EXPECT_EQ( pop1, 2.0 );
        EXPECT_FALSE( fifo.closed() );
        EXPECT_FALSE( fifo.empty() );
        EXPECT_FALSE( fifo.full() );
    }
    {
        auto pop1 = fifo.read_blockingly();
        EXPECT_EQ( pop1, 3.0 );
        EXPECT_FALSE( fifo.closed() );
        EXPECT_TRUE( fifo.empty() );
        EXPECT_FALSE( fifo.full() );
    }
}

TEST( fifo_guard, some_tests )
{
    using fifo_t = tz::object_fifo< int >;
    using guard_t = tz::fifo_guard< int >;

    {
        guard_t guard;
        EXPECT_FALSE( guard.fifo );
    }

    {
        auto fifo = std::make_shared< fifo_t >();
        EXPECT_FALSE( fifo->closed());
        {
            guard_t guard( fifo );
        }
        EXPECT_TRUE( fifo->closed() );
    }
    {
        auto fifo = std::make_shared< fifo_t >();
        EXPECT_FALSE( fifo->closed());
        {
            guard_t guard( fifo );
            guard.release();
        }
        EXPECT_FALSE( fifo->closed() );
    }
    {
        auto fifo = std::make_shared< fifo_t >();
        EXPECT_FALSE( fifo->closed());
        {
            guard_t guard_a( fifo );
            guard_t guard_b;
            guard_a.release();
            EXPECT_FALSE( fifo->closed());
            guard_b.adopt( std::move( guard_a ));
            EXPECT_FALSE( fifo->closed());
        }
        EXPECT_FALSE( fifo->closed() );
    }
    {
        auto fifo = std::make_shared< fifo_t >();
        EXPECT_FALSE( fifo->closed());
        {
            guard_t guard_a( fifo );
            guard_t guard_b;
            EXPECT_FALSE( fifo->closed());
            guard_b.adopt( std::move( guard_a ));
            EXPECT_FALSE( fifo->closed());
        }
        EXPECT_TRUE( fifo->closed() );
    }
    {
        auto fifo = std::make_shared< fifo_t >();
        auto guard_writing = tz::fifo_guard_writing< int >( fifo );
        EXPECT_FALSE( fifo->closed());
        {
            guard_t guard_b;
            EXPECT_FALSE( fifo->closed());
            guard_b.adopt( guard_writing.make_guard() );
            EXPECT_FALSE( fifo->closed());
        }
        EXPECT_TRUE( fifo->closed() );
    }
    {
        auto fifo = std::make_shared< fifo_t >();
        auto guard_writing = tz::fifo_guard_writing< int >( fifo );
        EXPECT_FALSE( fifo->closed());
        {
            guard_t guard_b;
            EXPECT_FALSE( fifo->closed());
            guard_b.adopt( std::move( guard_writing.make_guard() ));
            EXPECT_FALSE( fifo->closed());
        }
        EXPECT_TRUE( fifo->closed() );
    }
    {
        auto fifo = std::make_shared< fifo_t >();
        EXPECT_FALSE( fifo->closed());
        {
            guard_t guard_a( fifo );
            {
                guard_t guard_b;
                guard_b.participate( std::move( guard_a ));
                EXPECT_FALSE( fifo->closed());
            }
            EXPECT_TRUE( fifo->closed() );
        }
        EXPECT_TRUE( fifo->closed() );
    }

}
