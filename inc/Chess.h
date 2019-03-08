#pragma once

#include <vector>

// Chess interface like five in row, and put board and pieces in one
// class

class CChess
{
protected:
	//Piece type
	typedef enum tagPlayer
	{
		BLANK,
		BLACK,
		WHITE
	}PLAYER;
private:
	// Board size
	unsigned int m_nSize;
	// The length of each frame
	unsigned int m_nCellLen;
	// If it is holding a piece
	bool m_bHold;
	// Current player
	PLAYER m_enNow;
	// Record piece coordinates in placing order
	std::vector<POINT> m_vPiecePoint;
	// Record the whole board layout
	PLAYER** m_Board;
	// The bitmap of the pieces and board
	CBitmap m_bmpChess;
	// The bitmap of holding a piece. It is shown only when holding a
	// piece, so it is a dynamic allocation class.
	CBitmap* m_lpbmpHold;
protected:
	// The bitmap of the board, excluding the pieces
	CBitmap m_bmpBoard;
protected:
	// Store the moves in a array in order
	PLAYER** GetChessArray();
	// Get current player
	PLAYER GetNowPlayer();
	// Get the last move
	POINT GetLastPoint();
public:
	// The default size is 13x13
	CChess(unsigned int nSize = 13);
	~CChess();
	// Get the board size, the default shape is square
	unsigned int GetSize();
	// Set background of the board
	void SetBackground(CBitmap* lpBitmap);
	// Create the board
	virtual bool Create();
	// Show the board and pieces
	bool Show(CDC* pDC, int x, int y);
	// Get bitmap infomation of the board
	void GetBitmap(BITMAP* bmpInfo);
	// Test if the point is near a place can be put a piece, if so,
	// the function will return a recent coordinates.
	virtual bool GetPiont(int x, int y, POINT& Point);
	// Begin to hold
	void BeginHold();
	// Wether to draw a piece on (x, y) of the board bitmap according
	// to m_bHold
	bool HoldPiece(int x, int y);
	// Release a holding action
	void ReleasePiece();
	// Place a piece
	virtual bool PlacePiece(POINT Point);
	// The other one turn
	void ChangePlayer();
	// Take back one move
	bool WithdrawPiece();
};

