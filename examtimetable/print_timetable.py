from __future__ import print_function


def print_timetable(array_exams):
    print("Summary")
    print("=======")

    for i in range(array_exams.contents.size):
        print("Exam %d -> timeslot %d" % i + 1, array_exams.contents.data[i].contents.timeslot)


def print_detailed_timetable(c_array_exams, max_timeslot):
    print("Detailed Timetable")
    print("==================")

    for i in range(max_timeslot):
        print("Timeslot %d" % (i + 1))
        print("------------\n")

        for j in range(c_array_exams.contents.size):
            if (c_array_exams.contents.data[j].contents.timeslot == i):
                print("  Exam %d :" % (j + 1))
                print("      -> Prof : %d" % c_array_exams.contents.data[j].contents.teacher_id)

                print("      -> Timeslots available : (", end="")

                for k in range(max_timeslot):
                    print("%d " % c_array_exams.contents.data[j].contents.availabilities[k], end="")

                print(")")

                print("      -> Conflicts detected : (", end="")

                for k in range(c_array_exams.contents.size):
                    print("%d " % c_array_exams.contents.data[j].contents.conflicts[k], end="")

                print(")\n")

                print("      -> Students :")

                for k in range(c_array_exams.contents.data[j].contents.enrollment):
                    print("            %d" % c_array_exams.contents.data[j].contents.students[k])

                print("      -> Room : %u" % c_array_exams.contents.data[j].contents.room_id)
