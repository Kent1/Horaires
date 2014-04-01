class Room(object):

    def __init__(self, id, name, faculty, capacity, room_type, exams=None):
        """
        :param int id: Room id.
        :param str name: Name of the room.
        :param int faculty: Id of the faculty which owns the room.
        :param int capacity: Maximum seating capacity.
        :param int room_type: room_type id.
        :param list exams: list of exam for each timeslot.
        """
        self.id = id
        self.name = name
        self.faculty = faculty
        self.capacity = capacity
        self.room_type = room_type
        self.exams = exams or []


class RoomType(object):

    def __init__(self, name):
        self.name = name
