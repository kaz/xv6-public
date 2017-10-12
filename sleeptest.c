#include "types.h"
#include "stat.h"
#include "user.h"
#include "date.h"

// NOTE: This doesn't work if sec is large enough to change date
void addsec(struct rtcdate *d, uint sec) {
  d->second += sec;
  d->minute += d->second / 60;
  d->second %= 60;
  d->hour += d->minute / 60;
  d->minute %= 60;
  d->hour %= 24;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf(2, "usage: %s sec\n", argv[0]);
    exit();
  }
  struct rtcdate d;
  getdate(&d);
  printf(1, "%d:%d:%d\n", d.hour, d.minute, d.second);
  addsec(&d, atoi(argv[1]));
  sleep_until(&d);
  getdate(&d);
  printf(1, "%d:%d:%d\n", d.hour, d.minute, d.second);
  exit();
}
