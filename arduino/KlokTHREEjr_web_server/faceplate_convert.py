import sys
import os.path

h = sys.argv[1]
bak_fn = h + '.bak'

VAR = h.upper().replace('.', '_')
var = VAR[:-2].lower()

### use the backup if it exists
if os.path.exists(bak_fn):
    d = open(bak_fn).read().strip()
else:
    d = open(h).read().strip()
    bak = file=open(bak_fn, 'w')
    print(d, file=bak)
    print ('wrote backup', bak_fn)

subs = {'static const uint8_t WORDS[] PROGMEM': 'uint8_t {var}_words[]'.format(var=var),
        'static const uint8_t DISPLAYS[] PROGMEM': 'uint8_t {var}_displays[]'.format(var=var),
        'static const uint32_t MINUTE_LEDS[] PROGMEM': 'uint32_t {var}_minute_leds[]'.format(var=var),
        'static const uint32_t MINUTES_HACK[] PROGMEM': 'uint32_t {var}_minutes_hack[]'.format(var=var)}

for was in subs:
    d = d.replace(was, subs[was])

template = '''#ifndef {VAR}
#define {VAR}

#include "Faceplate.h"

{body}

static Faceplate {var}({var}_words, {var}_displays, {var}_minute_leds, {var}_minutes_hack);

#endif'''


d = template.format(body=d, VAR=VAR, var=var)

print(d, file=open(h, 'w'))

        
        
