import unittest
from models.task import Task


class TestSetDueDate(unittest.TestCase):

    def setUp(self):
        """Set up a Task instance."""
        self.task = Task("demo", "hello", None)

    def test_success_set_due_date(self):
        """Sets due_date to given value."""
        self.task.update_due_date("2026-01-01")
        self.assertEqual(self.task.due_date, "2026-01-01")

    def test_none_keeps_existing_due_date(self):
        """None leaves due_date unchanged."""
        self.task.update_due_date("2026-01-01")
        self.task.update_due_date(None)
        self.assertEqual(self.task.due_date, "2026-01-01")

    def test_empty_string_keeps_existing_due_date(self):
        """Empty string leaves due_date unchanged."""
        self.task.update_due_date("2026-01-01")
        self.task.update_due_date("")
        self.assertEqual(self.task.due_date, "2026-01-01")
