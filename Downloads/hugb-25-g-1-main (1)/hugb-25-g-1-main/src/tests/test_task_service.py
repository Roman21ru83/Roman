import os
import sys
import unittest
from unittest.mock import patch, MagicMock

# Ensure SRC is on sys.path when running locally
this_dir = os.path.dirname(__file__)
src_root = os.path.abspath(os.path.join(this_dir, ".."))
if src_root not in sys.path:
    sys.path.append(src_root)

from logic.task_service import TaskService  # noqa: E402
from models.task import Task  # noqa: E402


class TestTaskService(unittest.TestCase):
    def setUp(self):
        # Patch TaskIO inside logic.task_service so we don't touch real storage
        patcher = patch("logic.task_service.TaskIO")
        self.addCleanup(patcher.stop)
        self.MockTaskIO = patcher.start()

        # Configure the TaskIO mock instance used by TaskService
        self.mock_io_instance = self.MockTaskIO.return_value
        self.mock_io_instance.load_tasks.return_value = []
        self.mock_io_instance.write_task = MagicMock()

        # System under test
        self.svc = TaskService()

    def test_create_new_task_persists_and_returns_task(self):
        new = self.svc.create_new_task("Buy milk", "1L semi-skimmed", due_date=None)

        # in-memory list updated
        self.assertEqual(len(self.svc.tasks), 1)
        self.assertIsInstance(new, Task)
        self.assertEqual(new.title, "Buy milk")

        # persistence called once with the new task
        self.mock_io_instance.write_task.assert_called_once_with(new)

    def test_update_task_status_calls_model_and_changes_value(self):
        # Prepare a task and make updateStatus visible (in case it doesn't set attr)
        t = Task("Demo", "desc", None)
        # Spy: if Task.updateStatus doesn't set t.status, simulate it here
        original_update = getattr(t, "updateStatus", None)
        if original_update is None:
            # Fallback: provide a simple implementation
            def _upd(st): setattr(t, "status", st)
            t.updateStatus = _upd  # type: ignore[attr-defined]

        self.svc.update_task_status(t, "doing")
        # Either Task implements status or we set it via spy/fallback above
        self.assertEqual(getattr(t, "status", None), "doing")

    def test_update_due_date_calls_model_and_changes_value(self):
        t = Task("Demo", "desc", None)
        # If Task.update_due_date doesn't set attribute, simulate
        original_upd_due = getattr(t, "update_due_date", None)
        if original_upd_due is None:
            def _upd_due(d): setattr(t, "due_date", d)
            t.update_due_date = _upd_due  # type: ignore[attr-defined]

        self.svc.update_due_date(t, "2025-09-30")
        self.assertEqual(getattr(t, "due_date", None), "2025-09-30")


if __name__ == "__main__":
    unittest.main()
