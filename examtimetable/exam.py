class Exam(object):

    def __init__(self, id, name, faculty, professor, room_type,
                 students=None, room=None, availabilities=None,
                 conflicts=None, dependencies=None):
        self.id = id
        self.name = name
        self.faculty = faculty
        self.room_type = room_type
        self.professor = professor
        self.room = None
        self.timeslot = None
        self.students = students or []
        self.availabilities = availabilities or []
        self.conflicts = conflicts or []
        self.dependencies = dependencies or []
