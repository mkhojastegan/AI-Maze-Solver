#include"player.h"
#include"entity.h"
#include"stack.h"
#include"dllist.h"
#include"room.h"
#include"maze.h"
#include<iostream>

//  Player( ... )
//		Constructs and initializes the Player/Entity and its member variables
//		Remembers and discovers the starting room.
Player::Player(Maze* maze, Room p, std::string name, char sprite, const bool tbacktrack)
	:Entity(maze, p, name, sprite),
	m_lookingPaper(),
	m_discoveredRooms(),
	m_btQueue(),
	m_btStack(),
	BACKTRACKENABLED(tbacktrack) {

	// Discover the starting room
	m_discoveredRooms.insert_front(p);
	m_lookingPaper.enqueue(p);
}

//  stuck() 
//		See if the player is stuck in the maze (no solution)
bool Player::stuck() const {
	return Entity::state() == State::NOEXIT;
}

//  foundExit()
//		See if the player has found the exit
bool Player::foundExit() const {
	return Entity::state() == State::EXIT;
}

//  getTargetRoom()
//		Get the room the player wants to look around next.
//      If m_lookingPaper is empty then return an invalid room.
Room Player::getTargetRoom() const {
	if (m_lookingPaper.empty()) return Room(-1, -1);
	return m_lookingPaper.peek();
}

// discovered
//		returns true if the item is in the list
bool Player::discovered(const Room &p) const {
	return (m_discoveredRooms.find(p) != -1);
}



// say()
//		What does the player say?
//		Already Implemented, nothing to do here
void Player::say() {

	// Freedom supercedes being eaten
	if (state() == State::EXIT) {
		std::cout << name() << ": WEEEEEEEEE!";
		return;
	} 
 
	// Being eaten supercedes being lost
	switch (interact()) {
	case Interact::ATTACK:
		std::cout << name() << ": OUCH!";
		break;
	case Interact::GREET:
		break;
	case Interact::ALONE:
	default:
		switch (state()) {
		case State::LOOK:
			std::cout << name() << ": Where is the exit?";
			break;
		case State::NOEXIT:
			std::cout << name() << ": Oh no! I am Trapped!";
			break;
		case State::BACKTRACK:
			std::cout << name() << ": Got to backtrack...";
			break;
		default:
			break;
		}

		break;
	}
}


//  update() 
// 
//		I WORKED ON THIS WITH RYAN
// 
//		This function implements an algorithm to look through the maze
//      for places to move (STATE::LOOK). update also handles moving the 
//		player as well, if there is an open undiscovered cell to move to the  
//		player just moves there.  However, there will be cases in the next 
//		desired room to look around is not adjacent (more than one cell away), 
//		peek this room the player must backtrack (State::BACKTRACK) to a room 
//		that is adjacent to the next room the player was planning to visit
//
//		The player can only do one thing each call, they can either LOOK or 
//		BACKTRACK not both.  Nor should the player move more than one cell per
//		call to update.  If you examine the function calls for the entire game
//		you should observe that this is already be called within a loop. Your
//		Implementation should NOT have any loops that pertain to actual movement 
//		of the player.  
//
//		Backtracking is challenging, save it for the very very very last thing.
//		Make sure the STATE::LOOK aspect compiles and works first.
void Player::update() {
	// I WORKED ON THIS WITH RYAN

	say();

	if (BACKTRACKENABLED) {
	// Set by the settings file, if BACKTRACKENABLED is false, then
	// your program should behave exactly as seen in the slides or
	// example executables (with teleporting).
	// You may have multiple branching statements like this.
	// if(BACKTRACKENABLED) { ... code relating to backtracking 


		if (state() == State::LOOK) {
			m_discoveredRooms.insert_rear(room());

			if (m_lookingPaper.empty()){
				state(State::NOEXIT);
				stuck();
				say();
				return;
			}

			move(getTargetRoom());
			m_lookingPaper.dequeue();

			if (maze()->foundExit(room())) {
				state(State::EXIT);
				say();
				return;
			}

			Room west{ room().x() - 1, room().y() };
			if ((!discovered(west)) && maze()->open(west)) {
				m_discoveredRooms.insert_rear(west);
				m_lookingPaper.enqueue(west);
			}

			Room east{ room().x() + 1, room().y() };
			if ((!discovered(east)) && maze()->open(east)) {
				m_discoveredRooms.insert_rear(east);
				m_lookingPaper.enqueue(east);
			}

			Room north{ room().x(), room().y() - 1 };
			if ((!discovered(north)) && maze()->open(north)) {
				m_discoveredRooms.insert_rear(north);
				m_lookingPaper.enqueue(north);
			}

			Room south{ room().x(), room().y() + 1 };
			if ((!discovered(south)) && maze()->open(south)) {
				m_discoveredRooms.insert_rear(south);
				m_lookingPaper.enqueue(south);
			}

			if (m_lookingPaper.empty()){
				state(State::NOEXIT);
				stuck();
				say();
				return;
			}

			else
				state(State::BACKTRACK);

			return;
		}
		else {
			if (room().adjacent(getTargetRoom())){
				state(State::LOOK);
				move(getTargetRoom());

				if (maze()->foundExit(room())){
					state(State::EXIT);
					say();
					return;
				}
				return;
			}

			for (int i = 0; i < m_discoveredRooms.count(); i++)
				if (room().adjacent(m_discoveredRooms.peek(i))){
					move(m_discoveredRooms.peek(i));
					m_discoveredRooms.insert_rear(m_discoveredRooms.peek(i));
					m_discoveredRooms.remove_index(i);
					return;
				}

			return;
		}
	
	}
	else {

		m_discoveredRooms.insert_rear(room());

		if (m_lookingPaper.empty())
		{
			// Why would you put us in a maze we can't finish?
			// That's not fair!
			state(State::NOEXIT);
			stuck();
			say();
			return;
		}

		move(getTargetRoom());
		m_lookingPaper.dequeue();

		if (maze()->foundExit(room())){
			// Go go go before there's more sharks
			state(State::EXIT);
			say();
			return;
		}

		Room west{ room().x() - 1, room().y() };
		if ((!discovered(west)) && maze()->open(west)) {
			m_discoveredRooms.insert_rear(west);
			m_lookingPaper.enqueue(west);
		}

		Room east{ room().x() + 1, room().y() };
		if ((!discovered(east)) && maze()->open(east)) {
			m_discoveredRooms.insert_rear(east);
			m_lookingPaper.enqueue(east);
		}

		Room north{ room().x(), room().y() - 1 };
		if ((!discovered(north)) && maze()->open(north)) {
			m_discoveredRooms.insert_rear(north);
			m_lookingPaper.enqueue(north);
		}

		Room south{ room().x(), room().y() + 1 };
		if ((!discovered(south)) && maze()->open(south)) {
			m_discoveredRooms.insert_rear(south);
			m_lookingPaper.enqueue(south);
		}

		return;
	}
	}
