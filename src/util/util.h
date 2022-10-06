void initRandomness(void);
long randBound(long min, long max);
long randVal(long val);
int didCollide(const Entity *const e,
               const Entity *const e2);
void calcSlope(int x1, int y1,
               int x2, int y2,
               float *dx, float *dy);
void centerBulletTexture(Entity *const bullet,
                         const Entity *const entity);
void setBulletFireType(Entity *const enemy,
                       const Entity *const player);