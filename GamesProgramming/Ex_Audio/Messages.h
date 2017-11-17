#ifndef TANKMESSAGE_H
#define TANKMESSAGE_H

/** A position update message. */
struct PlayerMessage {
	/** The object number of this Player within the game world. */
	int id;

	/** The coordinates of this Player within the game world. */
	float x, y;

	/** The time at which this message was sent.
	    (Not the time at which it was received!) */
	float time;
};

#endif
