#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT	100
#define CMD_ADD		200
#define CMD_BUY		300
#define CMD_RENT	400
#define CMD_RETURN	500

#define MAXN		100
#define MAXL		8

extern void init(int N, int durableTime, int deliveryTimes[MAXN]);
extern void addBicycle(int cTimestamp, int pID, int bicycleNum);
extern void buyTicket(int cTimestamp, char uName[MAXL], int validTime);
extern int rentBicycle(int cTimestamp, char uName[MAXL], int pID);
extern int returnBicycle(int cTimestamp, char uName[MAXL], int pID);

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static int mSeed;
static int pseudo_rand(void)
{
	mSeed = mSeed * 246247151 + 9543169;
	return (mSeed >> 16) & 0x7FFFFFFF;
}

static bool run()
{
	int Q;
	int N, durableTime, deliveryTimes[MAXN], cTimestamp, pID, bicycleNum, validTime;
	int res = -1, ans;

	char uName[MAXL];

	int dT;

	scanf("%d %d", &Q, &mSeed);

	bool okay = false;

	if (mSeed == 0)
	{
		for (int q = 0; q <= Q; ++q)
		{
			int cmd;
			scanf("%d", &cmd);

			switch (cmd)
			{
			case CMD_INIT:
				scanf("%d %d", &N, &durableTime);
				for (int i = 0; i < N; ++i)
					scanf("%d", &deliveryTimes[i]);
				init(N, durableTime, deliveryTimes);
				okay = true;
				break;
			case CMD_ADD:
				scanf("%d %d %d", &cTimestamp, &pID, &bicycleNum);
				if (okay)
					addBicycle(cTimestamp, pID, bicycleNum);
				break;
			case CMD_BUY:
				scanf("%d %s %d", &cTimestamp, uName, &validTime);
				if (okay)
					buyTicket(cTimestamp, uName, validTime);
				break;
			case CMD_RENT:
				scanf("%d %s %d", &cTimestamp, uName, &pID);
				if (okay)
					res = rentBicycle(cTimestamp, uName, pID);
				scanf("%d", &ans);
				if (res != ans)
					okay = false;
				break;
			case CMD_RETURN:
				scanf("%d %s %d", &cTimestamp, uName, &pID);
				if (okay)
					res = returnBicycle(cTimestamp, uName, pID);
				scanf("%d", &ans);
				if (res != ans)
					okay = false;
				break;
			}
		}
	}
	else
	{
		scanf("%d", &dT);

		cTimestamp = 0;

		for (int q = 0; q <= Q; ++q)
		{
			int cmd;
			scanf("%d", &cmd);

			switch (cmd)
			{
			case CMD_INIT:
				scanf("%d %d", &N, &durableTime);
				for (int i = 0; i < N; ++i)
					scanf("%d", &deliveryTimes[i]);
				init(N, durableTime, deliveryTimes);
				okay = true;
				break;
			case CMD_ADD:
				cTimestamp += pseudo_rand() % dT + 1;
				scanf("%d %d", &pID, &bicycleNum);
				if (okay)
					addBicycle(cTimestamp, pID, bicycleNum);
				break;
			case CMD_BUY:
				cTimestamp += pseudo_rand() % dT + 1;
				scanf("%s %d", uName, &validTime);
				if (okay)
					buyTicket(cTimestamp, uName, validTime);
				break;
			case CMD_RENT:
				cTimestamp += pseudo_rand() % dT + 1;
				scanf("%s %d", uName, &pID);
				if (okay)
					res = rentBicycle(cTimestamp, uName, pID);
				scanf("%d", &ans);
				if (res != ans)
					okay = false;
				break;
			case CMD_RETURN:
				cTimestamp += pseudo_rand() % dT + 1;
				scanf("%s %d", uName, &pID);
				if (okay)
					res = returnBicycle(cTimestamp, uName, pID);
				scanf("%d", &ans);
				if (res != ans)
					okay = false;
				break;
			}
		}
	}

	return okay;
}

int main()
{
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++)
	{
		int score = run() ? MARK : 0;

		printf("#%d %d\n", tc, score);
	}

	return 0;
}