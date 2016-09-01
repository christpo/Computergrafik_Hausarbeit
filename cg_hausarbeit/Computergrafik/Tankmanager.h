#include "tank.h"
#include <time.h>
#include <math.h>

class Tankmanager
{
public:
	Tankmanager();
	~Tankmanager();
	void setStartPointTank(int point);
	int getStartPointTank();
	void setEnemyCount(int count);
	int getEnemyCount();
	void setMaxRange(int range);
	int getMaxRange();
	void setMaxX(int x);
	int getMaxX();
	void setInvert(bool invert);
	bool getInvert();

	bool seeTheEnemy(const Vector& userPosition, int count);
	bool shootRange(const Vector& userPosition, int count);
	void driving(const Vector& userPosition, float Deltatime, int numberOfWaves);
	bool load(const char* ChassisModel, const char* CononModel, const Vector& StartPos, int count);
	void aim(int X, int Z, int count);
	void ausrichten();
	void randomSpeedInitialized();
	void update(float deltatime);
	void startReset();
	bool allEnemysHit();
	int giveRandomXPosition();
	vector<Tank*> getEnemys();

private:
	int maxRange = 10;
	int startPointTank;
	int enemyCount;
	bool invert = true;
	int maxX = 5;
	vector<Tank*> enemys;
	vector<float> randomSpeed;
	vector<int> xPositions;
};

