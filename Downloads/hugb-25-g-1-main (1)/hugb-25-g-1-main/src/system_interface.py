from logic.task_service import TaskService


class SystemComm:
    def __init__(self, service: TaskService):
        self.service = service

    def create_task(self, title: str, desc: str, due_date: str = None):
        return self.service.create_new_task(title, desc, due_date)
