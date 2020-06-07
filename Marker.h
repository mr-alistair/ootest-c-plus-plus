#pragma once
using namespace std;
#include <string>

class Marker {

public:

	//properties
	int m_owner;

	int m_location;

	//methods
	//CONSTRUCTOR
	
	Marker()
	{
		//nothing
	}

	Marker(int x_id)
	{

			this->m_owner = x_id;

			this->m_location = 1;
	}

	void m_assignowner( int x_owner ) {

		this->m_owner = x_owner;
				
	}

	int m_calclocation(int x_diceroll) {

		int x_calcvalue = this->m_location * x_diceroll;

		return x_calcvalue;

	}

	void m_setlocation(int x_newlocation) {

		this->m_location = x_newlocation;
	}

	int m_get_location() {

		return this->m_location;

	}

	bool m_get_status() {

		if (this->m_location == 120)
		{
			return 0;
		}
		else {
			return 1;
		}


	}

};

