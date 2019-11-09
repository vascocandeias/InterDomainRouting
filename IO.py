def read(command):
	"""Gets the input value from the user"""

	userInput = None
	while userInput is None:
		try:
			userInput = input(command)
		except IOError :
			print("There was an error reading the value")
			continue
		except :
			print("\nSomething happened. Exiting...")
			exit()

	return userInput

