from jinja2 import Environment, FileSystemLoader, select_autoescape

env = Environment(
    loader=FileSystemLoader('templates/')
)
template = env.get_template('main_page.html')
out_file = template.render(test='Peta')

with open("realMainPage.html", "w") as fh:
    fh.write(out_file)