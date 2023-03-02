function matlab2vtk(m, filename)

f = fopen(filename, 'w');

% Header info
fprintf(f, '# vtk DataFile Version 3.0\n');
fprintf(f, 'Matlab output\n');
fprintf(f, 'ASCII\n');
fprintf(f, 'DATASET STRUCTURED_POINTS\n');
fprintf(f, 'DIMENSIONS %d %d %d\n', size(m,1), size(m,2), size(m,3));
fprintf(f, 'ORIGIN %f %f %f\n', -(size(m,1) - 1) / 2, -(size(m,2) - 1) / 2, -(size(m,3) - 1) / 2);
fprintf(f, 'SPACING %f %f %f\n', 1.0, size(m,1) / size(m,2), size(m,1) / size(m,3));
fprintf(f, 'POINT_DATA %d\n', size(m,1) * size(m,2) * size(m,3));
fprintf(f, 'SCALARS data float 1\n');
fprintf(f, 'LOOKUP_TABLE default\n');

% Data
fprintf(f, '%f ', m);

fclose(f);