#define EXCLUDE_MAIN
#define MOVIE_TIME 1

#include <stdio.h>

#include "main.cpp"

const int MAP_WIDTH = 16000;
const int MAP_HEIGHT = 9000;
const int SCALE = 10;

const int PP = 3;
const unsigned int width = MAP_WIDTH / SCALE;
const unsigned int height = MAP_HEIGHT / SCALE;
constexpr unsigned int imageSize = width * height * 3;
unsigned char image[imageSize];

const float bgColor[PP] = {0.0, 0.0, 0.0};
const float cpColor[PP] = {1.0, 1.0, 1.0};

const float colors[POD_COUNT][PP] = {
  {1.0f, 0.0f, 0.0f},
  {1.0f, 1.0f, 0.0f},
  {0.0f, 1.0f, 0.0f},
  {0.0f, 0.0f, 1.0f}
};

void writePPM(const char* filename, unsigned int width, unsigned int height) {
  int dimx = width, dimy = height;
  FILE *fp = fopen(filename, "wb"); /* b - binary mode */
  (void) fprintf(fp, "P6\n%d %d\n255\n", dimx, dimy);
  fwrite(image, imageSize, 1, fp);
  (void) fclose(fp);
}

void writeImageTest(int n, float t) {

  constexpr int FNAME_LEN = 256;
  char fname[FNAME_LEN];
  // output better be a ramdisk. Use ImDisk.
  snprintf(fname, FNAME_LEN, "/mnt/g/out/csb%05d.ppm", n);

  //generate some image
  for(unsigned y = 0; y < height; y++) {
    for(unsigned x = 0; x < width; x++) {
      float r = (float) x / (float) height;
      float g = (float) y / (float) width;
      float b = sin(r + t) + sin(g + t);
      r = max(0.0f, min(1.0f, r));
      g = max(0.0f, min(1.0f, g));
      b = max(0.0f, min(1.0f, b));
      image[3 * width * y + 3 * x + 0] = (int) (255.0 * r);
      image[3 * width * y + 3 * x + 1] = (int) (255.0 * g);
      image[3 * width * y + 3 * x + 2] = (int) (255.0 * b);
      //image[4 * width * y + 4 * x + 3] = 255;
    }
  }

  writePPM(fname, width, height);
}

void setPixel(int x, int y, float r, float g, float b) {
  if (x < 0 || x >= (int) width || y < 0 || y >= (int) height) {
    return;
  }
  r = max(0.0f, min(1.0f, r));
  g = max(0.0f, min(1.0f, g));
  b = max(0.0f, min(1.0f, b));
  image[PP * width * y + PP * x + 0] = (int) (255.0 * r);
  image[PP * width * y + PP * x + 1] = (int) (255.0 * g);
  image[PP * width * y + PP * x + 2] = (int) (255.0 * b);
}

void getPixel(int x, int y, float& r, float &g, float& b) {
  if (x < 0 || x >= (int) width || y < 0 || y >= (int) height) {
    r = 0.0;
    g = 0.0;
    b = 0.0;
    return;
  }
  r = image[PP * width * y + PP * x + 0] / 255.0f;
  g = image[PP * width * y + PP * x + 1] / 255.0f;
  b = image[PP * width * y + PP * x + 2] / 255.0f;
}

void initImage() {
  for(unsigned y = 0; y < height; y++) {
    for(unsigned x = 0; x < width; x++) {
      setPixel(x, y, 0.0, 0.0, 0.0);
    }
  }
}

void drawCircle(int x, int y, int radius, const float color[PP]) {
  float r = color[0];
  float g = color[1];
  float b = color[2];

  x /= SCALE;
  y /= SCALE;
  radius /= SCALE;

  for (int dx = -radius; dx <= radius; dx++) {
    for (int dy = -radius; dy <= radius; dy++) {
      if (dx * dx + dy * dy <= radius * radius) {
        setPixel(x + dx, y + dy, r, g, b);
      }
    }
  }
}

void drawCircleBorder(int x, int y, int minRadius, int maxRadius, const float color[PP]) {
  float r = color[0];
  float g = color[1];
  float b = color[2];

  x /= SCALE;
  y /= SCALE;
  minRadius /= SCALE;
  maxRadius /= SCALE;

  int radius = maxRadius;

  for (int dx = -radius; dx <= radius; dx++) {
    for (int dy = -radius; dy <= radius; dy++) {
      int pt = dx * dx + dy * dy;
      if (minRadius * minRadius <= pt && pt <= radius * radius) {
        setPixel(x + dx, y + dy, r, g, b);
      }
    }
  }
}

void fade() {
  const float f = 0.0;
  for(unsigned y = 0; y < height; y++) {
    for(unsigned x = 0; x < width; x++) {
      float r, g, b;
      getPixel(x, y, r, g, b);
      setPixel(x, y, r * f, g * f, b * f);
    }
  }
}

void solCallback(Solution* sol, float score) {
  for (int i = 0; i < POD_COUNT; i++) {
    const Pod& pod = *pods[i];

    drawCircle(pod.x, pod.y, 3, colors[i]);
  }
}

void drawCheckPoints() {
  for (int i = 0; i < cp_ct; i++) {
    Checkpoint& cp = *cps[i];
    drawCircleBorder(cp.x, cp.y, cp.r * 0.95, cp.r, cpColor);
  }
}

void beginRoundCallback() {
  fade();

  drawCheckPoints();

  for (int i = 0; i < POD_COUNT; i++) {
    const Pod& pod = *pods[i];

    drawCircleBorder(pod.x, pod.y, pod.r * 0.95, pod.r, colors[i]);
  }
}

void endRoundCallback() {

}

int main() {

    callback = &solCallback;

    center = new Checkpoint(0, 16000 / 2, 9000 / 2);

    cin >> laps >> cp_ct;
    for (int i = 0; i < cp_ct; i++) {
        int cx, cy;
        cin >> cx >> cy;
        cps[i] = new Checkpoint(i, cx, cy);
    }

    for (int i = 0; i < 4; i++) pods[i] = new Pod(i);

    pods[0]->partner = pods[1];
    pods[1]->partner = pods[0];
    pods[2]->partner = pods[3];
    pods[3]->partner = pods[2];

    ReflexBot me_reflex;

    SearchBot opp(2);
    opp.oppBots.push_back(&me_reflex);

    SearchBot me;
    me.oppBots.push_back(&opp);

    constexpr int FNAME_LEN = 256;
    char fname[FNAME_LEN];
    // output better be a ramdisk. Use ImDisk.

    initImage();

    while (1) {
        r++;

        for (int i = 0; i < 4; i++) {
            int x, y, vx, vy, angle, ncpid;
            cin >> x >> y >> vx >> vy >> angle >> ncpid;
            if (r == 0 && i > 1 && angle > -1) is_p2 = true;
            pods[i]->update(x, y, vx, vy, angle, ncpid);
        }
        beginRoundCallback();

        // if (r > 20) {
        //   cerr << "R > 20" << endl;
        //   break;
        // }

        if (cin.eof()) {
          cerr << "EOF" << endl;
          break;
        }

        now = high_resolution_clock::now();

        float time_limit = r ? 0.070 : 0.98;
        //time_limit *= 0.3;

        // use this to test reflex bot behavior
        // me_reflex.move_as_main();

        opp.solve(time_limit*0.15);
        me.solve(time_limit, r > 0);

        endRoundCallback();

        snprintf(fname, FNAME_LEN, "/mnt/g/out/csb%05d.ppm", r);
        writePPM(fname, width, height);

        if (r > 0) cerr << "Avg iters: " << sols_ct / r << "; Avg sims: " << sols_ct*DEPTH / r << endl;

        print_move(me.sol.thrusts[0], me.sol.angles[0], pods[0]);
        print_move(me.sol.thrusts[DEPTH], me.sol.angles[DEPTH], pods[1]);
    }
}
