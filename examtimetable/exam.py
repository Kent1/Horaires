class Exam(object):

    def __init__(self, id, name, faculty, room_type, teacher,
                 student=[], availabilities=[], dependencies=[]):
        self.id = id
        self.name = name
        self.faculty = faculty
        self.room_type = room_type
        self.teacher = teacher
        self.room = None
        self.timeslot = None
        self.students = student or []
        self.availabilities = availabilities or []
        self.dependencies = dependencies or []
