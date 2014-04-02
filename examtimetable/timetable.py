import schedule


class Timetable(object):

    def __init__(self, timeslots, exams={}, rooms={}):
        """
        :param int timeslots: Maximum timeslots available.
        :param dict exams: Exams dictionnary. Contains exams mapped with id.
        :param dict rooms: Rooms dictionnary. Contains rooms mapped with id.
        """
        self.timeslots = timeslots
        self.exams = exams or {}
        self.rooms = rooms or {}

    def print_timetable(self):
        """Display timetable."""
        print("Timetable")
        print("=========")

        for timeslot in range(self.timeslots):
            self.print_timeslot(timeslot)

    def print_timeslot(self, timeslot):
        """Display all exams given a timeslot.
        :param int timeslot: timeslot to display.
        """
        print "timeslot %d" % timeslot
        print "----------"
        print
        for exam in self.exams.values():
            if exam.timeslot == timeslot:
                print("  Exam %d :" % exam.id)
                print("\t-> Prof : %d" % exam.professor)
                print("\t-> Timeslots available : %s" % exam.availabilities)
                print("\t-> Conflicts detected : %s" % exam.conflicts)
                print("\t-> Students : %s" % exam.students)
                print("\t-> Room : %d" % exam.room.id)
        print

    schedule = schedule.schedule
