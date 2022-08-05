# Name: Dennis Preston Beaty
#netid: dbeaty
# IMPORTANT NOTICE
# -------------------------------------------------------
# This was written using python 3.9 this version uses the
# commnad called is_alive() for threading rather than
# previous versions isAlive() please take notice of this
# when running this program
import logging
import threading
import time
import random


class Runner(threading.Thread):
    # Create a runner with their number
    def __init__(self, number):
        self.number = number
        # variables to keep track of
        self.section = 1
        self.time_of_lap = 0
        self.time_to_finish = 0
        self.hold_time = 0
        threading.Thread.__init__(self)
    # Run race for the current runner
    def run(self):
        for x in range(4):
            self.hold_time = time.time()    
            random_num = random.uniform(10, 100)
            time.sleep(random_num)
            self.time_of_lap = random_num
            self.time_to_finish += random_num
            self.section += 1


class Reporter(threading.Thread):
    # class to print to reporter updates
    def printer(self):
        # open file to write to
        temp = time.time()
        self.file.write("Results at time:" + str(self.time_to_disp) + "\n")

        self.file.write("----------------------------------------------------------\n")
        self.file.write("| Contestant  # | Section | Section Time | Overall Time |\n")
        for x in self.active_runners:
            # Write each name
            self.file.write("| Contestant " + str(x.number).zfill(3))
            if x.section > 4:
                # write for it is finished
                self.file.write(" | End | ")
                self.file.write("{:.8f}".format(x.time_of_lap).ljust(12) + " | ")
                self.file.write("{:.8f}".format(x.time_to_finish).ljust(12) + " |\n")
            else:
                self.file.write(" | " + str(x.section) + " | ")
                subtract_by = temp
                x.time_of_lap = (subtract_by-x.hold_time)
                self.file.write("{:.8f}".format(x.time_of_lap).ljust(12) + " | ")
                self.file.write("{:.8f}".format(temp - self.race_time).ljust(12) + " |\n")
                # write normal instance
                self.file.write("----------------------------------------------------------\n")

# INITALIZE THE REPORTER VAR
def __init__(self, f):
    # variables to keep track of
    self.active_runners = []
    self.finished_runners = []
    self.time_to_disp = 0
    threading.Thread.__init__(self)
    self.file = f

# CODE RAN ON THREAD OF REPORTER
def run(self):
    # initalize and fill your list of runners
    for x in range(1, 1000):
        self.active_runners.append(Runner(x))
    self.race_time = time.time()
    # create list of threads for all active runners
    for x in self.active_runners:
        x.start()
    self.printer()

    # wait for all runners to finish the race before proceeding with next code
    while (len(self.active_runners) != 0):
        for x in self.active_runners:
            if not x.is_alive():
                self.finished_runners.append(x)
                self.active_runners.remove(x)
        time.sleep(2)
        self.time_to_disp += 2
        if (len(self.active_runners) != 0):
            self.printer()
        # stop printing once all runners are finished
        self.file.close()
        # sort finished runners
        self.finished_runners.sort(key=lambda x: x.time_to_finish)
        # print final values to FinalResults.txt
        f = open("FinalResults.txt", "w+")
        f.write("---------------------------------\n")
        f.write("| Contestant # | Overall Time |\n")
        total_time = 0
        for x in self.finished_runners:
            # write contestant number
            f.write("| Contestant " + str(x.number).zfill(3) + " | ")
            # write time to finishe
            f.write("{:.8f}".format(x.time_to_finish).ljust(12) + " |\n")
            total_time += x.time_to_finish
        f.write("---------------------------------\n")
        f.write("Overall, the runners spent " + str(round((total_time), 8)) + "seconds running total.\n")
        f.close()

def main():
    f = open("ReporterUpdates.txt", "w+")
    # create a reporter
    reporter = Reporter(f)
    # run the race and write details to output files
    reporter.start()

if __name__ == "__main__":
    main()
