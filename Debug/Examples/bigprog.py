while 1:
	choice = 0
	num1 = 0
	num2 = 0
	print "What would you like to do?"
	print "(1) Addition, (2) Subtraction, (3) Multiplication, (4) Division, (5) Sum of numbers"
	choice = int(raw_input())
	if choice == 1 :
		print "Input the two numbers"
		num1 = int(raw_input())
		num2 = int(raw_input())
		result = num1 + num2
		print "Num1 + Num2 = ",result
	elif choice == 2 :
		print "Input the two numbers"
		num1 = int(raw_input())
		num2 = int(raw_input())
		result = num1 - num2
		print "Num1 - Num2 = ",result
	elif choice == 3 :
		print "Input the two numbers"
		num1 = int(raw_input())
		num2 = int(raw_input())
		result = num1 * num2
		print "Num1 * Num2 = ",result
	elif choice == 4 :
		print "Input the two numbers"
		num1 = int(raw_input())
		num2 = int(raw_input())
		result = num1 / num2
		print "Num1 / Num2 = ",result
	elif choice == 5 :
		print "Input the number you want to sum up to"
		num1 = int(raw_input())
		total = 0
		for i in range(1,num1):
			total = total + i
		print "The sum of numbers up to ",num1," is ",total
