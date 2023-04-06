import os
import subprocess


menu_text = [
    'Menu:',
    '1. Add value',
    '2. Remove last value',
    '3. Print size and capacity',
    '4. Print vector',
    '5. Exit',
    'Enter choice:'
]

size_capacity_text = [
    'Size: {0}',
    'Capacity: {0}'
]

enter_value_to_add_text = 'Enter value to add:'
value_added_text = 'Value added.'
last_value_removed_text = 'Last value removed.'
vector_is_empty_text = 'Vector is empty.'
exiting_text = 'Exiting...'
vector_contents_text = 'Vector contents:'
invalid_choice_text = 'Invalid choice.'


def start(command, envs=dict()):
    cmd_env = os.environ.copy()
    cmd_env.update(envs)
    process = subprocess.Popen(command, shell=False,
                               stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE, env=cmd_env)
    return process


def terminate(process):
    process.stdin.close()
    process.stdout.close()
    process.stderr.close()
    process.terminate()
    process.wait(timeout=0.1)


def assert_text_output(text, output):
    assert text == output, f'{output} is not equal to {text}'


def readline(process):
    return process.stdout.readline().decode('utf-8').strip()


def read_menu(process):
    for text in menu_text:
        output = readline(process)
        assert_text_output(text, output)


def read_add(process):
    output = readline(process)
    assert_text_output(enter_value_to_add_text, output)


def read_value_added(process):
    output = readline(process)
    assert_text_output(value_added_text, output)


def read_remove(process):
    output = readline(process)
    assert_text_output(last_value_removed_text, output)


def read_empty(process):
    output = readline(process)
    assert_text_output(vector_is_empty_text, output)


def read_exit(process):
    output = readline(process)
    assert_text_output(exiting_text, output)


def read_size_capacity(process, size, capacity):
    output = readline(process)
    text = size_capacity_text[0].format(size)
    assert_text_output(text, output)
    output = readline(process)
    text = size_capacity_text[1].format(capacity)
    assert_text_output(text, output)


def read_vector_contents(process, values):
    output = readline(process)
    assert_text_output(vector_contents_text, output)
    text = ' '.join(map(str, values))
    output = readline(process)
    assert_text_output(text, output)


def read_invalid_choice(process):
    output = readline(process)
    assert_text_output(invalid_choice_text, output)


def send_value(process, message):
    process.stdin.write(f'{message}\n'.encode('utf-8'))
    process.stdin.flush()
