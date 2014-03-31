class Room(object):

    def __init__(self, id, name, faculty, capacity, room_type, exams=[]):
        self.id = id
        self.name = name
        self.faculty = faculty
        self.capacity = capacity
        self.room_type = room_type
        self.exams = exams or []


class RoomType(object):

    def __init__(self, name):
        self.name = name
