import unittest
from unittest.mock import patch
from system_interface import SystemComm
from logic.task_service import TaskService


class TestCreateTask(unittest.TestCase):

    def setUp(self):
        """Mock TaskIO so no files are read or written."""
        self.patcher = patch("logic.task_service.TaskIO")
        MockTaskIO = self.patcher.start()
        self.addCleanup(self.patcher.stop)

        io = MockTaskIO.return_value
        io.load_tasks.return_value = []
        io.write_task.return_value = None

        self.iface = SystemComm(TaskService())

    def test_success_create_task(self):
        """Creates a task, status starts as 'todo'."""
        t = self.iface.create_task("demo", "hello", "2025-10-01")
        d = t.to_dict()
        self.assertEqual(d["title"], "demo")
        self.assertEqual(d["desc"], "hello")
        self.assertEqual(d["due_date"], "2025-10-01")
        self.assertEqual(d["status"], "todo")

    def test_empty_title(self):
        """Empty title is currently allowed."""
        t = self.iface.create_task("", "desc ok", None)
        self.assertEqual(t.status, "todo")

    def test_no_due_date(self):
        """Due_date is optional (None accepted)."""
        t = self.iface.create_task("x", None, None)
        self.assertIsNone(t.due_date)
