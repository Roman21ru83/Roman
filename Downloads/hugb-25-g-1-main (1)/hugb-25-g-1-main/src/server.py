from flask import Flask, request, jsonify
from logic.task_service import TaskService
from system_interface import SystemComm

app = Flask(__name__)

service = TaskService()
iface = SystemComm(service)


@app.route('/tasks', methods=['POST'])
def create_task():
    """"Creates a new task"""
    data = request.get_json()
    title = data.get("title")
    desc = data.get("desc")
    due_date = data.get("due_date")

    task = iface.create_task(title, desc, due_date)
    response = jsonify(task.to_dict())
    response.status_code = 201
    return response


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5001, debug=True)
