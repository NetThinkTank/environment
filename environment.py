from settings import *
import json, requests, serial, sys, time


# from https://stackoverflow.com/questions/39644638/how-to-take-the-nth-digit-of-a-number-in-python

def get_digit(number, n):
	return number // 10**n % 10


# STRIP FUNCTIONS

def strip_off(sc, delay = True):
	if not sc and not strip_debug:
		return

	command = chr(32) + chr(127)

	if strip_active and sc:
		sc.write(command)

	if delay:
		time.sleep(strip_delay)


def strip_led(sc, led_color, led_number):
	if not sc and not strip_debug:
		return

	command = chr(34) + chr(led_color) + chr(led_number) + chr(127)

	if strip.active and sc:
		sc.write(command)

	time.sleep(strip_delay)


def strip_range(
	sc,
	start_led_1, start_led_2, start_led_3,
	end_led_1, end_led_2, end_led_3,
	increment,
	colors,
	delay = True
):
	if not sc and not strip_debug:
		return

	if strip_debug:
		print('STRIP RANGE')

		print('START_LED_1: ', start_led_1)
		print('START_LED_2: ', start_led_2)
		print('START_LED_3: ', start_led_3)

		print('END_LED_1: ', end_led_1)
		print('END_LED_2: ', end_led_2)
		print('END_LED_3: ', end_led_3)

		print('INCREMENT: ', increment)
		print('COLORS: ', colors)
		print('DELAY: ', delay)

	command = \
		chr(35) + \
		chr(start_led_1) + chr(start_led_2) + chr(start_led_3) + \
		chr(end_led_1) + chr(end_led_2) + chr(end_led_3) + \
		chr(increment)

	for c in colors:
		command += chr(c)

	command += chr(127)

	if strip_active and sc:
		sc.write(command)

	if delay:
		time.sleep(strip_delay)


# MAIN SCRIPT

sc = None

start_led_1 = get_digit(start_led, 2)
start_led_2 = get_digit(start_led, 1)
start_led_3 = get_digit(start_led, 0)

end_led_1 = get_digit(end_led, 2)
end_led_2 = get_digit(end_led, 1)
end_led_3 = get_digit(end_led, 0)

if strip_active:
	try:
		sc = serial.Serial('/dev/ttyACM' + strip1_port, 9600)
	except:
		print('ERROR OPENING ARDUINO')
		exit()

time.sleep(1)

for i in range(0, 3):
	if sc:
		strip_off(sc)

	time.sleep(strip_delay)

while True:
	try:
		data = json.loads(requests.get(data_url).text)
		colors = data['colors']
	except:
		print('CONNECTION ERROR')
		sys.exit()

	print('COLORS: ', data['colors'])

	strip_off(sc)

	strip_range(
		sc,
		start_led_1, start_led_2, start_led_3,
		end_led_1, end_led_2, end_led_3,
		increment,
		colors,
		True
	)

	time.sleep(60)
