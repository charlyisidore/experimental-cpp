#include "signal.hpp"
#include <iostream>

struct Receiver
{
	void update()
	{
		std::cout << "Update" << std::endl;
	}

	void updateValue( int x )
	{
		std::cout << "Update value: " << x << std::endl;
	}

	void updateValues( int x, int y )
	{
		std::cout << "Update values: " << x << ' ' << y << std::endl;
	}
};

struct OtherReceiver
{
	void update()
	{
		std::cout << "Other Update" << std::endl;
	}

	void updateValue( int x )
	{
		std::cout << "Other Update value: " << x << std::endl;
	}

	void updateValues( int x, int y )
	{
		std::cout << "Other Update values: " << x << ' ' << y << std::endl;
	}
};

class Sender
{
public:
	signal::_0           actionPerformed;
	signal::_1<int>      valueChanged;
	signal::_2<int, int> valuesReset;

	Sender() : x( 0 ), y( 0 ) {}

	void someAction()
	{
		std::cout << "Hello!" << std::endl;
		actionPerformed(); // emit
	}

	void changeValue( int value )
	{
		x = value;
		valueChanged( x ); // emit
	}

	void resetValues()
	{
		x = 0;
		y = 0;
		valuesReset( x, y ); // emit
	}

private:
	int x, y;
};

int main()
{
	Sender        sender;
	Receiver      receiver;
	OtherReceiver otherReceiver;

	// Syntax: sender . signal . connect < ReceiverClass, &Method > ( &receiverInstance );

	sender.actionPerformed.connect< Receiver,      &Receiver::update      >( &receiver );
	sender.actionPerformed.connect< OtherReceiver, &OtherReceiver::update >( &otherReceiver );

	sender.valueChanged.connect< Receiver,      &Receiver::updateValue      >( &receiver );
	sender.valueChanged.connect< OtherReceiver, &OtherReceiver::updateValue >( &otherReceiver );

	sender.valuesReset.connect< Receiver,      &Receiver::updateValues      >( &receiver );
	sender.valuesReset.connect< OtherReceiver, &OtherReceiver::updateValues >( &otherReceiver );

	sender.changeValue( 5 );
	sender.someAction();
	sender.resetValues();
	return 0;
}

