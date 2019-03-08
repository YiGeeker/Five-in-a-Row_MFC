#include "StdAfx.h"
#include "FIR.h"

#include <vector>

CFIR::CFIR(unsigned int nSize) :CChess(nSize)
{
	m_nSize = CChess::GetSize();
	m_Board = CChess::GetChessArray();
	m_bPlayerFirst = true;
}

CFIR::~CFIR()
{
}

unsigned int CFIR::GetDirNum(int dx, int dy, POINT Point, PLAYER Player)
{
	int x, y;
	unsigned int nCount = 0;

	x = Point.x;
	y = Point.y;
	// Ensure that the array is not out of bounds
	while (x + dx >= 0 && x + dx < (int)m_nSize && y + dy >= 0 && y + dy < (int)m_nSize)
	{
		x += dx;
		y += dy;
		if (m_Board[x][y] == Player) nCount++;
		else break;
	}
	return nCount;
}

unsigned int CFIR::GetMaxLineNum(POINT Point, PLAYER Player)
{
	unsigned int LineNum[4];
	unsigned int i, nMax = 1;

	// East-west direction
	LineNum[0] = this->GetDirNum(-1, 0, Point, Player) + this->GetDirNum(1, 0, Point, Player) + 1;
	// Northwest to southeast direction
	LineNum[1] = this->GetDirNum(-1, -1, Point, Player) + this->GetDirNum(1, 1, Point, Player) + 1;
	// North-south direction
	LineNum[2] = this->GetDirNum(0, -1, Point, Player) + this->GetDirNum(0, 1, Point, Player) + 1;
	// Northeastern southwest
	LineNum[3] = this->GetDirNum(1, -1, Point, Player) + this->GetDirNum(-1, 1, Point, Player) + 1;

	for (i = 0; i < 4; i++)
	{
		if (LineNum[i] > nMax) nMax = LineNum[i];
	}

	return nMax;
}

bool CFIR::IsLineLive(int dx, int dy, POINT Point, PLAYER Player, unsigned int& nNum)
{
	int i, j;
	int x, y;
	unsigned int nCount = 1, nSum = 0;
	std::vector<unsigned int> SumList;
	PLAYER OppPlayer;

	nNum = 1;
	if (Player == PLAYER::BLACK)
	{
		OppPlayer = PLAYER::WHITE;
	}
	else
	{
		OppPlayer = PLAYER::BLACK;
	}

	x = Point.x;
	y = Point.y;
	// Ensure that the array is not out of bounds
	while (x + dx >= 0 && x + dx < (int)m_nSize && y + dy >= 0 && y + dy < (int)m_nSize)
	{
		if (m_Board[x + dx][y + dy] != OppPlayer) nCount++;
		else break;
		x += dx;
		y += dy;
	}
	// Check another direction
	x = Point.x;
	y = Point.y;
	// Ensure that the array is not out of bounds
	while (x - dx >= 0 && x - dx < (int)m_nSize && y - dy >= 0 && y - dy < (int)m_nSize)
	{
		if (m_Board[x - dx][y - dy] != OppPlayer) nCount++;
		else break;
		x -= dx;
		y -= dy;
	}
	if (nCount < 5) return false;

	// Calculate the number of this kind of piece on this line
	for (i = 0; (unsigned int)i < nCount - 4; i++)
	{
		nSum = 0;
		for (j = 0; j < 5; j++)
		{
			if (m_Board[x + dx*j][y + dy*j] == Player) nSum++;
			// Assume this point is placed a piece
			else if ((Point.x == x + dx*j) && (Point.y == y + dy*j)) nSum++;
		}
		x += dx;
		y += dy;
		SumList.push_back(nSum);
	}

	// Record the maximum number of SumList
	nNum = SumList[0];
	for (i = 0; (unsigned int)i < SumList.size(); i++)
	{
		if (SumList[i] > nNum) nNum = SumList[i];
	}

	// Record the number of maximum number
	nCount = 0;
	for (i = 0; (unsigned int)i < SumList.size(); i++)
	{
		if (SumList[i] == nNum) nCount++;
	}

	if (nCount > 1) return true; // Live
	else return false;			 // Dead
}

void CFIR::GetPointModel(POINTMODEL* PointModel)
{
	unsigned int nNum;

	// The rest arrays should be initialize to zero
	ZeroMemory(PointModel->Model, sizeof(PointModel->Model));

	// West-east direction
	if (this->IsLineLive(1, 0, PointModel->Point, PointModel->Player, nNum))
	{
		PointModel->Model[nNum - 1][0]++;
	}
	else
	{
		PointModel->Model[nNum - 1][1]++;
	}
	// Northwest to southeast direction 
	if (this->IsLineLive(1, 1, PointModel->Point, PointModel->Player, nNum))
	{
		PointModel->Model[nNum - 1][0]++;
	}
	else
	{
		PointModel->Model[nNum - 1][1]++;
	}
	// North-south direction
	if (this->IsLineLive(0, 1, PointModel->Point, PointModel->Player, nNum))
	{
		PointModel->Model[nNum - 1][0]++;
	}
	else
	{
		PointModel->Model[nNum - 1][1]++;
	}
	// Northeast to southwest direction
	if (this->IsLineLive(1, -1, PointModel->Point, PointModel->Player, nNum))
	{
		PointModel->Model[nNum - 1][0]++;
	}
	else
	{
		PointModel->Model[nNum - 1][1]++;
	}
}

POINT CFIR::GetComputerPosition()
{
	unsigned int i, j, k, l;
	PLAYER Player, OppPlayer;
	POINT Point;
	// Our score and the opposed value
	unsigned int **MyValue, **OpponentValue;
	POINTMODEL PointModel;
	
	// The first index indicates the number of piece and the second
	// index indicates the type either live type or punching type

	// The score value: live one, punching one, live two, punching
	// two, live three, punching three, live four, punching four,
	// five, five
	unsigned int MValue[5][2] = { { 40,30 },{ 80,50 },{ 800,100 },{ 20000,1000 },{ 1000000,1000000 } };
	unsigned int OValue[5][2] = { { 30,20 },{ 50,40 },{ 400,70 },{ 5000,500 },{ 100000,100000 } };

	// Used to compare score of two sides and calculate the the
	// closest point to the center
	unsigned int nMyMax = 0, nOpponentMax = 0;
	// The coordinates of the centeral point
	POINT CentralPoint;
	// The length to the center
	unsigned int d = 0, dTemp = 0;
	std::vector<POINT> vMaxPoint;

	// Set from the computer side
	if (m_bPlayerFirst)
	{
		Player = PLAYER::WHITE;
		OppPlayer = PLAYER::BLACK;
	}
	else
	{
		Player = PLAYER::BLACK;
		OppPlayer = PLAYER::WHITE;
	}

	// Initialize the score array of two sides to zero
	MyValue = new unsigned int*[m_nSize];
	OpponentValue = new unsigned int*[m_nSize];
	for (i = 0; i < m_nSize; i++)
	{
		MyValue[i] = new unsigned int[m_nSize]();
		OpponentValue[i] = new unsigned int[m_nSize]();
	}

	// Calculate score of two sides
	for (i = 0; i < m_nSize; i++)
	{
		for (j = 0; j < m_nSize; j++)
		{
			Point.x = i;
			Point.y = j;
			// Check next point if this point has a piece
			if (m_Board[i][j] != PLAYER::BLANK) continue;
			PointModel.Point = Point;
			// Calculate score of our side
			if (!this->IsForbidden(Point, Player))
			{
				PointModel.Player = Player;
				this->GetPointModel(&PointModel);
				for (k = 0; k < 5; k++)
				{
					for (l = 0; l < PointModel.Model[k][0]; l++) MyValue[i][j] += MValue[k][0];
					for (l = 0; l < PointModel.Model[k][1]; l++) MyValue[i][j] += MValue[k][1];
				}

				// Get extra score if it is two three or three four
				if (PointModel.Model[2][0] == 2 || (PointModel.Model[2][0] == 1 && PointModel.Model[3][1] == 1)) MyValue[i][j] += 2200;
				if (PointModel.Model[3][1] == 2) MyValue[i][j] += 10000;
			}

			// Calculate opposed score
			if (!this->IsForbidden(Point, OppPlayer))
			{
				PointModel.Player = OppPlayer;
				PointModel.Point = Point;
				this->GetPointModel(&PointModel);
				for (k = 0; k < 5; k++)
				{
					for (l = 0; l < PointModel.Model[k][0]; l++) OpponentValue[i][j] += OValue[k][0];
					for (l = 0; l < PointModel.Model[k][1]; l++) OpponentValue[i][j] += OValue[k][1];
				}

				// Get extra score if it is two three or three four
				if (PointModel.Model[2][0] == 2 || (PointModel.Model[2][0] == 1 && PointModel.Model[3][1] == 1)) OpponentValue[i][j] += 4000;
				if (PointModel.Model[3][1] == 2) OpponentValue[i][j] += 4000;
			}
		}
	}

	nMyMax = 0;
	nOpponentMax = 0;

	for (i = 0; i < m_nSize; i++)
	{
		for (j = 0; j < m_nSize; j++)
		{
			// Calculate the maximum score of our side
			if (MyValue[i][j]>nMyMax) nMyMax = MyValue[i][j];
			// Calculate the maximum score of the oppoesd sid
			if (OpponentValue[i][j]>nOpponentMax) nOpponentMax = OpponentValue[i][j];
		}
	}

	if (nMyMax >= nOpponentMax)
	{
		for (i = 0; i < m_nSize; i++)
		{
			for (j = 0; j < m_nSize; j++)
			{
				// Calculate the coordinates of our side
				if (MyValue[i][j] == nMyMax)
				{
					Point.x = i;
					Point.y = j;
					vMaxPoint.push_back(Point);
				}
			}
		}
	}
	else
	{
		for (i = 0; i < m_nSize; i++)
		{
			for (j = 0; j < m_nSize; j++)
			{
				// the coordinates of our side
				if (OpponentValue[i][j] == nOpponentMax)
				{
					Point.x = i;
					Point.y = j;
					vMaxPoint.push_back(Point);
				}
			}
		}
	}

	// Find out the closed point to the center
	CentralPoint.x = (m_nSize - 1) / 2;
	CentralPoint.y = (m_nSize - 1) / 2;
	d = (vMaxPoint[0].x - CentralPoint.x)*(vMaxPoint[0].x - CentralPoint.x) + (vMaxPoint[0].y - CentralPoint.y)*(vMaxPoint[0].y - CentralPoint.y);
	for (i = 1; i < vMaxPoint.size(); i++)
	{
		dTemp = (vMaxPoint[i].x - CentralPoint.x)*(vMaxPoint[i].x - CentralPoint.x) + (vMaxPoint[i].y - CentralPoint.y)*(vMaxPoint[i].y - CentralPoint.y);
		if (dTemp < d)
		{
			d = dTemp;
			Point.x = vMaxPoint[i].x;
			Point.y = vMaxPoint[i].y;
		}
	}

	for (i = 0; i < m_nSize; i++)
	{
		delete[] OpponentValue[i];
		delete[] MyValue[i];
	}
	delete[] OpponentValue;
	delete[] MyValue;

	return Point;
}

void CFIR::SetComputerFirst()
{
	m_bPlayerFirst = false;
}

bool CFIR::IsForbidden(POINT Point, PLAYER Player)
{
	POINTMODEL PointModel;

	// The white side donnot have a forbidden
	if (Player == PLAYER::WHITE)
	{
		return false;
	}

	PointModel.Point = Point;
	PointModel.Player = PLAYER::BLACK;
	this->GetPointModel(&PointModel);

	// Two live three but not five connected will be forbidden
	if (PointModel.Model[2][0] == 2 && (PointModel.Model[4][0] + PointModel.Model[4][1] == 0)) return true;
	// Live four and punching four but not five connected will be forbidden
	if ((PointModel.Model[3][0] + PointModel.Model[3][1] == 2) && (PointModel.Model[4][0] + PointModel.Model[4][1] == 0)) return true;
	return false;
}

bool CFIR::IsForbidden(POINT Point)
{
	if (this->IsForbidden(Point, CChess::GetNowPlayer())) return true;
	return false;
}

bool CFIR::IsWin()
{
	if (this->GetMaxLineNum(CChess::GetLastPoint(), CChess::GetNowPlayer()) >= 5) return true;
	return false;
}
