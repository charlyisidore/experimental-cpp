/*
	A minimal signal/slot implementation example
*/

#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <list>

namespace signal
{

/*
	Slot classes
*/

class slot_0
{
public:
	// Define the type of the static method "method_callback"
	typedef void (*callback_type)(void *);

	// No-argument initializer
	slot_0() : _object( 0 ), _callback( 0 ) {}

	// Invoker method
	void operator () () const
	{
		return (*_callback)( _object );
	}

	// Factory static method
	template<class C, void (C::*Method)()>
	static slot_0 create( C * object )
	{
		return slot_0( object, &method_callback<C, Method> );
	}

private:
	// Private contructor used by factory method
	template<class C>
	slot_0( C * object, callback_type callback ) : _object( object ), _callback( callback ) {}

	// The static invokers, generated at compile-time for each type "C" and method "Method"
	template <class C, void (C::*Method)()>
	static void method_callback( void * object )
	{
		return ( static_cast<C *>( object ) ->*Method )();
	}

	void          * _object;     // Pointer to object instance
	callback_type   _callback;   // Pointer to static invoker
};

template<typename Arg1>
class slot_1
{
public:
	typedef void (*callback_type)(void *, Arg1);

	slot_1() : _object( 0 ), _callback( 0 ) {}

	void operator () ( Arg1 a1 ) const
	{
		return (*_callback)( _object, a1 );
	}

	template<class C, void (C::*Method)(Arg1)>
	static slot_1 create( C * object )
	{
		return slot_1( object, &method_callback<C, Method> );
	}

private:
	template<class C>
	slot_1( C * object, callback_type callback ) : _object( object ), _callback( callback ) {}

	template <class C, void (C::*Method)(Arg1)>
	static void method_callback( void * object, Arg1 a1 )
	{
		return ( static_cast<C *>( object ) ->*Method )( a1 );
	}

	void          * _object;
	callback_type   _callback;
};

template<typename Arg1, typename Arg2>
class slot_2
{
public:
	typedef void (*callback_type)(void *, Arg1, Arg2);

	slot_2() : _object( 0 ), _callback( 0 ) {}

	void operator () ( Arg1 a1, Arg2 a2 ) const
	{
		return (*_callback)( _object, a1, a2 );
	}

	template<class C, void (C::*Method)(Arg1, Arg2)>
	static slot_2 create( C * object )
	{
		return slot_2( object, &method_callback<C, Method> );
	}

private:
	template<class C>
	slot_2( C * object, callback_type callback ) : _object( object ), _callback( callback ) {}

	template <class C, void (C::*Method)(Arg1, Arg2)>
	static void method_callback( void * object, Arg1 a1, Arg2 a2 )
	{
		return ( static_cast<C *>( object ) ->*Method )( a1, a2 );
	}

	void          * _object;
	callback_type   _callback;
};

/*
	Signal classes
*/

// Signal base class encapsulates the slot container.
template<class Slot>
class signal_base
{
public:
	// Define alias for slot_*<...>
	typedef Slot slot_type;

protected:
	// Append a slot
	void _connect( const slot_type & s )
	{
		_slots.push_back( s );
	}

	// Iterate over slots to send the message
	template<class I>
	void _emit( const I & invoke ) const
	{
		for ( typename std::list<slot_type>::const_iterator it( _slots.begin() ); it != _slots.end(); ++it )
		{
			invoke( *it );
		}
	}

private:
	std::list<Slot> _slots;
};

class _0 : public signal_base< slot_0 >
{
public:
	typedef signal_base< slot_0 >        parent;
	typedef typename parent::slot_type   slot_type;

	template<class C, void (C::*Method)()>
	void connect( C * object )
	{
		parent::_connect( slot_type::create<C, Method>( object ) );
	}

	void operator () () const
	{
		_emit( invoke() );
	}

private:
	struct invoke
	{
		void operator () ( const slot_type & s ) const { s(); }
	};
};

template<typename Arg1>
class _1 : public signal_base< slot_1<Arg1> >
{
public:
	typedef signal_base< slot_1<Arg1> >   parent;
	typedef typename parent::slot_type    slot_type;

	template<class C, void (C::*Method)(Arg1)>
	void connect( C * object )
	{
		parent::_connect( slot_type::template create<C, Method>( object ) );
	}

	void operator () ( Arg1 a1 ) const
	{
		_emit( invoke( a1 ) );
	}

private:
	struct invoke
	{
		Arg1 _a1;
		invoke( Arg1 a1 ) :
			_a1( a1 ) {}
		void operator () ( const slot_type & s ) const { s( _a1 ); }
	};
};

template<typename Arg1, typename Arg2>
class _2 : public signal_base< slot_2<Arg1, Arg2> >
{
public:
	typedef signal_base< slot_2<Arg1, Arg2> >   parent;
	typedef typename parent::slot_type          slot_type;

	template<class C, void (C::*Method)(Arg1, Arg2)>
	void connect( C * object )
	{
		parent::_connect( slot_type::template create<C, Method>( object ) );
	}

	void operator () ( Arg1 a1, Arg2 a2 ) const
	{
		_emit( invoke( a1, a2 ) );
	}

private:
	struct invoke
	{
		Arg1 _a1;
		Arg2 _a2;
		invoke( Arg1 a1, Arg2 a2 ) :
			_a1( a1 ),
			_a2( a2 ) {}
		void operator () ( const slot_type & s ) const { s( _a1, _a2 ); }
	};
};

}

#endif
