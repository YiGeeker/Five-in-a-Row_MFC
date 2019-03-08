#pragma once
#include "chess.h"
class CFIR :
	public CChess
{
private:
	// Store the point mode
	typedef struct tagPointModel
	{
		POINT Point;
		PLAYER Player;
		// Model denotes the connected number and type of this
		// point. The first index indicates the number of the
		// connected pieces, and the second index indicates the type
		
		// For instance, Model[2][0] denotes a live tpye with 3
		// connected pieces and Model[3][1] denotes a punching type
		// with 4 connected pieces
		unsigned int Model[5][2];
	}POINTMODEL;
private:
	// Current board size
	unsigned int m_nSize;
	// Store the pieces on board
	PLAYER** m_Board;
	// The default value is player on the offensive
	bool m_bPlayerFirst;
private:
	// Calculate how many pieces in a row on the direction of (dx,
	// dy), excluding this point
	unsigned int GetDirNum(int dx, int dy, POINT Point, PLAYER Player);
	// Check if is live on the direction of (dx, dy), and calculate
	// the number of the check piece, not connected necessaryly,
	// including this piont
	bool IsLineLive(int dx, int dy, POINT Point, PLAYER Player, unsigned int& nNum);
	// Calculate the maximum number of connected pieces on each
	// direction of this point
	unsigned int GetMaxLineNum(POINT Point, PLAYER Player);
	// Get this point model
	void GetPointModel(POINTMODEL* PointModel);
	// Check if it is a forbidden
	bool IsForbidden(POINT Point, PLAYER Player);
public:
	CFIR(unsigned int nSize);
	~CFIR();
	// Check if it is a forbidden
	bool IsForbidden(POINT Point);
	// Check if someone wins
	bool IsWin();
	// Set computer is on the offensive
	void SetComputerFirst();
	// Calculate the best point for computer
	POINT GetComputerPosition();
};

