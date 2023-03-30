Appointment Organizer (APO)

1. Programming Language
	- C Language


2. Program Usage:

- For normal input

	## Compile the G05_APO.c file & get the G05_APO executable file (use -lm because we use math library in c file)
	gcc G05_APO.c -o G05_APO -lm 

	## Run G05_APO by:
	## 20230401 -> Start Date, 20230430 -> End Date, john mary lucy paul -> Users (!user can replace it)
	./G05_APO 20230401 20230430 john mary lucy paul


- For batch file input

	gcc G05_APO.c -o G05_APO -lm
	## 20230401 -> Start Date, 20230430 -> End Date, john mary lucy paul -> Users, G05_tests.dat -> the batch file which include the all the test inputs (!user can replace it)
	./G05_APO 20230401 20230430 john mary lucy paul < G05_tests.dat 


- Use 4 types of commands to schedule meeting

	## Example (privateTime): 
	## paul -> host user, 20230401 1800 -> start time, 2.0 -> time usage
	> privateTime paul 20230401 1800 2.0

	## Example (projectMeeting): 
	## lucy -> host user, 20230402 1900 -> start time, 2.0 -> time usage, paul mary -> other user who join this event
	> projectMeeting john 20230402 1900 2.0 paul mary

	## Example (groupStudy): 
	## lucy -> host user, 20230403 1800 -> start time, 2.0 -> time usage, john lucy -> other user who join this event
	> groupStudy paul 20230403 1800 2.0 john lucy

	## Example (gathering): 
	## lucy -> host user, 20230404 1900 -> start time, 4.0 -> time usage, john paul mary -> other user who join this event
	>  gathering lucy 20230404 1900 4.0 john paul mary


- Use printSchd to export report file (after inital the executable file)

	## Example: use FCFS (print the schedules use the FCFS algorithms)
	> printSchd FCFS
	## output sample:


	## Example: use PIORITY (print the schedules use the PIORITY algorithms)
	> printSchd PIORITY
	## output sample:


	## Example: use ALL (print the schedules use all the algorithms)
	> printSchd ALL
	## output sample:


3. COMP2432 Group Members (G05)

- DENG Chun Yung (21084115D)
- JIANG Guanlin (21093962D)
- LI Tong (21101988D)
- LIU Yuzhou (21100602D)
- NIYITEGEKA Berwa Aime Noel (21104645D)


The copyright is belong to COMP2432 Group 5 in 2023.

