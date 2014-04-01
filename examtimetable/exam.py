class Exam(object):

    def __init__(self, id, name, faculty, professor, room_type,
                 students=None, room=None, availabilities=None,
                 conflicts=None, dependencies=None):
        """Constructor.
        :param int id: The id of the exam.
        :param str name: The name of the exam.
        :param int faculty: The id of the faculty which organizes examen.
        :param int professor: The id of the professor who give courses.
        :param int room_type: The id of the room type.
        :param list students: List of students id who take the exam.
        :param Room room: The room in which the examination takes place
        :param list availabilities: An timeslot-length boolean array of availabilities.
        :param list conflicts: An boolean array of conflicts.
        :param list dependencies: A list of dépendencies.
        """
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
