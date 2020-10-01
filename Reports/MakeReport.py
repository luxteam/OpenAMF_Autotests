from jinja2 import Environment, FileSystemLoader, select_autoescape
import sys
import json

# Reading from all .json files with results from all GPUs (passed as arguments)
statistics = list()
for i in range(1, len(sys.argv)):
    results_json = sys.argv[i]
    with open(results_json, "r") as results_file:
        statistics.append(json.load(results_file))

env = Environment(
    loader=FileSystemLoader('templates/')
)

### Creating main page ###    
template = env.get_template('main_page.html')

# Forming information for 'Summary' section
gpu_stat = list()
for result in statistics:
    gpu_stat.append(
        {
        'GPU' : 'Radeon VII',
        'Configuration' : 'Static VS2017',
        'Total' : result['tests'],
        'Passed' : result['tests'] - result['failures'] - result['disabled'] - result['errors'],
        'Failed' : result['failures'],
        'Disabled' : result['disabled'],
        'Error' : result['errors'],
        'Time taken': result['time'],
        'Test suites': result['testsuites']
        }
    )
out_file = template.render(gpu_stat=gpu_stat)

with open("templates/realMainPage.html", "w") as fh:
    fh.write(out_file)


# Forming information for 'Details' sections
template = env.get_template('test_suite_details_page.html')
