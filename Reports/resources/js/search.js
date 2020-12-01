function search_case() {
    var input, filter, table, tr, td, i, txtValue;
    input = document.getElementById("search_case");
    filter = input.value.toUpperCase();
    table = document.getElementById("cases_table");
    tr = table.getElementsByTagName("tr");
    for (i = 0; i < tr.length; i++) {
      td = tr[i].getElementsByTagName("td")[0];
      if (td) {
        txtValue = td.textContent || td.innerText;
        if (txtValue.toUpperCase().indexOf(filter) > -1) {
          tr[i].style.display = "";
        } else {
          tr[i].style.display = "none";
        }
      }       
    }
  }