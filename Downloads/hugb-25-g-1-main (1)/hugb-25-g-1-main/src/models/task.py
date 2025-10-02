class Task:
    STATUS = ["todo", "doing", "done"]
    next_task_id = 0

    def __init__(self, title: str, desc: str, due_date: str = None):
        self.id = Task.next_task_id
        Task.next_task_id += 1
        self.title = title
        self.desc = desc
        self.status = "todo"
        self.due_date = due_date

    def __string__(self):
        return f"Task: {self.title}, Description: {self.desc}, \
            Due date: {self.due_date}"

    def updateStatus(self, status: int):
        """"Updates the status attribute of task instance"""
        if 0 <= status < len(self.STATUS):
            self.status = self.STATUS[status]

    def update_due_date(self, date: str) -> None:
        """Updates or sets the due_date attribute of task instance"""
        if date:
            self.due_date = date

    def to_dict(self) -> dict:
        """"Returns Task instance formatted as a dict"""
        return {
            "id": self.id,
            "title": self.title,
            "desc": self.desc,
            "due_date": self.due_date,
            "status": self.status
        }
