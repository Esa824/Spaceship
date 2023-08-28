typedef struct {
  int x, y;
} Enemy;

struct Controls {
  const char *up;
  const char *down;
  const char *left;
  const char *right;
  const char *shoot;
};

struct LevelData {
  int levelNumber;
  int timeLimit;
  int enemiesKilled;
  int totalEnemies;
  int healthUpPowerUp;
  int speedUpPowerUp;
};
