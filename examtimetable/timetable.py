import schedule


class Timetable(object):

    def __init__(self, timeslots, exams=[], rooms=[]):
        self.timeslots = timeslots
        self.exams = exams or []
        self.rooms = rooms or []

    def print_timetable(self):
        print("Summary")
        print("=======")

        for exam in self.exams:
            print("Exam %d -> timeslot %d" % exam.id, exam.timeslot)

    def schedule(self):
        schedule.schedule(self.timeslots, self.exams, self.rooms)
