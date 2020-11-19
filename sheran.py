import config

lines = config.display_size.lines 
colons = config.display_size.colons - 2

def start_display(lin, cln):
	work = True
	wag_lines = 0
	while work:
		if wag_lines == 0:
			print("####" + "SheranOS" + "#" * (cln - 14) + "###")
			wag_lines += 1
		else:
			if (wag_lines >= 1) and (wag_lines < (lin - 1)):
				print("#", end = "")
				print("." * (cln - 1), end = "")
				print("#")
				wag_lines += 1
			else:
				if(wag_lines == (lin - 1)):
					print("#" * cln + "#")
					wag_lines += 1
				else:
					if wag_lines == lin:
						work = False

start_display(lines, colons)