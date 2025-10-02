import unittest
from models.task import Task


class TestSetStatus(unittest.TestCase):

    def setUp(self):
        """Set up Task instance."""
        self.task = Task("demo", "hello", None)

    def test_success_number_one_sets_doing(self):
        """Sets status to 'doing' with 1."""
        self.task.updateStatus(1)
        self.assertEqual(self.task.status, "doing")

    def test_out_of_range_number(self):
        """Out of range number leaves status unchanged."""
        before = self.task.status
        self.task.updateStatus(99)
        self.assertEqual(self.task.status, before)

    def test_invalid_string(self):
        """Negative input leaves status unchanged."""
        before = self.task.status
        self.task.updateStatus(-1)
        self.assertEqual(self.task.status, before)

    def test_set_status_done(self):
        """Sets status to 'done' with 2."""
        self.task.updateStatus(2)
        self.assertEqual(self.task.status, "done")
