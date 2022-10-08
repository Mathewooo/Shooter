void initRandomness(void);
long randBd(long min, long max);
long randVl(long val);
long randS(long val);
int didCollide(const Entity *const e,
               const Entity *const e2);
void calcSlope(int x1, int y1,
               int x2, int y2,
               float *dx, float *dy);
void centerBulletTexture(Entity *const bullet,
                         const Entity *const entity);
void setBulletFireType(Entity *const enemy,
                       const Entity *const player);